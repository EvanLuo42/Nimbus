#include "RenderGraph.h"

#include <iostream>
#include <ranges>

#include "RenderPassNode.h"

#include "vk_mem_alloc.h"

#include <queue>

namespace Nimbus::RenderGraph
{
RenderGraph::RenderGraph() = default;

RenderGraph::~RenderGraph()
{
    for (const auto& [handle, imgData] : images)
    {
        if (allocations.contains(handle))
        {
            const auto alloc = allocations.at(handle);
            vmaDestroyImage(allocator, *imgData.image, alloc);
        }
    }
    allocations.clear();
    images.clear();
};

void RenderGraph::allocateResources(vk::raii::Device& device, vk::Extent2D defaultExtent)
{
    struct AllocInfo
    {
        ResourceHandle handle;
        ResourceDesc desc;
        ResourceLifetime lifetime;
    };

    std::vector<AllocInfo> allocList;
    allocList.reserve(resources.size());
    for (auto& [h, d] : resources)
        allocList.push_back({.handle = h, .desc = d, .lifetime = lifetimes[h]});

    std::ranges::sort(allocList, [](auto& a, auto& b) { return a.lifetime.firstUse < b.lifetime.firstUse; });

    std::unordered_map<ResourceHandle, ResourceHandle> aliasMap;

    for (auto& [handle, desc, lifetime] : allocList)
    {
        bool reused = false;

        for (auto& [existingHandle, existingDesc] : resources)
        {
            if (existingHandle == handle)
                continue;
            const auto& [firstUse, lastUse] = lifetimes[existingHandle];

            bool compatible = existingDesc.compatibleWith(desc);

            if (bool nonOverlapping = lastUse < lifetime.firstUse;
                compatible && nonOverlapping && images.contains(existingHandle))
            {
                std::cout << "[Alias] " << desc.name << " reuses image of " << existingDesc.name << "\n";
                images[handle] = std::move(images[existingHandle]);
                aliasMap[handle] = existingHandle;
                reused = true;
                break;
            }
        }

        if (reused)
            continue;

        vk::ImageCreateInfo info{
            .imageType = vk::ImageType::e2D,
            .format = desc.format,
            .extent = {.width = desc.extent.width ? desc.extent.width : defaultExtent.width,
                       .height = desc.extent.height ? desc.extent.height : defaultExtent.height,
                       .depth = 1},
            .mipLevels = 1,
            .arrayLayers = 1,
            .samples = vk::SampleCountFlagBits::e1,
            .tiling = vk::ImageTiling::eOptimal,
            .usage = desc.usage,
            .initialLayout = vk::ImageLayout::eUndefined,
        };

        VkImage rawImage{};
        VmaAllocationCreateInfo allocInfo = {};
        allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

        VmaAllocation allocation{};
        VmaAllocationInfo allocDetails{};
        if (vmaCreateImage(allocator, reinterpret_cast<const VkImageCreateInfo*>(&info), &allocInfo, &rawImage,
                           &allocation, &allocDetails) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to allocate image with VMA");
        }

        vk::raii::Image image{device, rawImage};

        vk::ImageViewCreateInfo viewInfo{.image = *image,
                                         .viewType = vk::ImageViewType::e2D,
                                         .format = desc.format,
                                         .subresourceRange = {.aspectMask = vk::ImageAspectFlagBits::eColor,
                                                              .baseMipLevel = 0,
                                                              .levelCount = 1,
                                                              .baseArrayLayer = 0,
                                                              .layerCount = 1}};
        vk::raii::ImageView view{device, viewInfo};

        images[handle] = {std::move(image), std::move(view)};
        allocations[handle] = allocation;
        std::cout << "[RenderGraph] Allocated: " << desc.name << "\n";
    }

    std::cout << "[RenderGraph] Final image map:\n";
    for (const auto& h : images | std::views::keys)
    {
        if (auto aliasIt = aliasMap.find(h); aliasIt != aliasMap.end())
            std::cout << "  " << resources[h].name << " → alias of " << resources[aliasIt->second].name << "\n";
        else
            std::cout << "  " << resources[h].name << " → owns image\n";
    }
}
ResourceHandle RenderGraph::getOrCreateTexture(const std::string& name, const ResourceDesc& desc)
{
    for (auto& [h, existing] : resources)
    {
        if (existing.name == name)
        {
            if (!existing.compatibleWith(desc))
            {
                std::cerr << "[RenderGraph][Warning] Resource " << name << " incompatible, creating new variant.\n";
                ResourceDesc newDesc = desc;
                newDesc.name = name + "_v" + std::to_string(nextId);
                const ResourceHandle newHandle = nextId++;
                resources[newHandle] = newDesc;
                lifetimes[newHandle] = {};
                return newHandle;
            }
            return h;
        }
    }

    const ResourceHandle handle = nextId++;
    resources[handle] = desc;
    lifetimes[handle] = {};
    std::cout << "[RenderGraph] Created new resource: " << desc.name << "\n";
    return handle;
}

void RenderGraph::setup()
{
    currentPassIndex = 0;
    for (const auto& pass : passes)
    {
        pass->setup(*this);
        ++currentPassIndex;
    }
    analyzeLifetimes();
}

void RenderGraph::compile(vk::raii::Device& device)
{
    analyzeLifetimes();
    allocateResources(device, vk::Extent2D{.width = 1280, .height = 720});
}

void RenderGraph::execute(vk::raii::CommandBuffer& cmd, vk::Extent2D extent)
{
    const auto order = topologicalSort();
    std::unordered_map<ResourceHandle, vk::ImageLayout> currentLayout;

    for (auto handle : images | std::views::keys)
        currentLayout[handle] = vk::ImageLayout::eUndefined;

    for (const size_t idx : order)
    {
        auto& pass = *passes[idx];

        for (auto input : pass.inputs)
        {
            if (const auto prev = currentLayout[input]; prev != vk::ImageLayout::eShaderReadOnlyOptimal)
            {
                transitionImage(cmd, input, prev, vk::ImageLayout::eShaderReadOnlyOptimal,
                                vk::PipelineStageFlagBits2::eAllGraphics, vk::PipelineStageFlagBits2::eFragmentShader,
                                vk::AccessFlagBits2::eColorAttachmentWrite, vk::AccessFlagBits2::eShaderRead);
                currentLayout[input] = vk::ImageLayout::eShaderReadOnlyOptimal;
            }
        }

        for (auto output : pass.outputs)
        {
            auto prev = currentLayout[output];
            if (prev != vk::ImageLayout::eColorAttachmentOptimal)
            {
                transitionImage(cmd, output, prev, vk::ImageLayout::eColorAttachmentOptimal,
                                vk::PipelineStageFlagBits2::eAllGraphics,
                                vk::PipelineStageFlagBits2::eColorAttachmentOutput, vk::AccessFlagBits2::eShaderRead,
                                vk::AccessFlagBits2::eColorAttachmentWrite);
                currentLayout[output] = vk::ImageLayout::eColorAttachmentOptimal;
            }
        }

        const bool hasOutput = !pass.outputs.empty();

        if (hasOutput)
            beginRenderPass(cmd, pass, extent);

        pass.execute(cmd);

        if (hasOutput)
            endRenderPass(cmd);

        for (auto output : pass.outputs)
        {
            transitionImage(cmd, output, currentLayout[output], vk::ImageLayout::eShaderReadOnlyOptimal,
                            vk::PipelineStageFlagBits2::eColorAttachmentOutput,
                            vk::PipelineStageFlagBits2::eFragmentShader, vk::AccessFlagBits2::eColorAttachmentWrite,
                            vk::AccessFlagBits2::eShaderRead);
            currentLayout[output] = vk::ImageLayout::eShaderReadOnlyOptimal;
        }
    }
}

std::vector<size_t> RenderGraph::topologicalSort() const
{
    const size_t n = passes.size();
    std::vector<std::vector<size_t>> adj(n);
    std::vector<int> indegree(n, 0);

    std::unordered_map<ResourceHandle, size_t> lastWriter;
    for (size_t i = 0; i < n; ++i)
    {
        for (auto h : passes[i]->outputs)
            lastWriter[h] = i;
    }

    for (size_t i = 0; i < n; ++i)
    {
        for (auto input : passes[i]->inputs)
        {
            if (auto it = lastWriter.find(input); it != lastWriter.end() && it->second != i)
            {
                adj[it->second].push_back(i);
                indegree[i]++;
            }
        }
    }

    std::queue<size_t> q;
    for (size_t i = 0; i < n; ++i)
        if (indegree[i] == 0)
            q.push(i);

    std::vector<size_t> order;
    while (!q.empty())
    {
        size_t u = q.front();
        q.pop();
        order.push_back(u);
        for (size_t v : adj[u])
        {
            if (--indegree[v] == 0)
                q.push(v);
        }
    }

    if (order.size() != n)
        throw std::runtime_error("[RenderGraph] Cycle detected in pass dependencies!");

    return order;
}

void RenderGraph::transitionImage(const vk::raii::CommandBuffer& cmd, const ResourceHandle handle,
                                  const vk::ImageLayout oldLayout, const vk::ImageLayout newLayout,
                                  const vk::PipelineStageFlags2 srcStage, const vk::PipelineStageFlags2 dstStage,
                                  const vk::AccessFlags2 srcAccess, const vk::AccessFlags2 dstAccess)
{
    const auto& image = images.at(handle).image;

    vk::ImageMemoryBarrier2 barrier{.srcStageMask = srcStage,
                                    .srcAccessMask = srcAccess,
                                    .dstStageMask = dstStage,
                                    .dstAccessMask = dstAccess,
                                    .oldLayout = oldLayout,
                                    .newLayout = newLayout,
                                    .image = *image,
                                    .subresourceRange = {.aspectMask = vk::ImageAspectFlagBits::eColor,
                                                         .baseMipLevel = 0,
                                                         .levelCount = 1,
                                                         .baseArrayLayer = 0,
                                                         .layerCount = 1}};

    const vk::DependencyInfo depInfo{.imageMemoryBarrierCount = 1, .pImageMemoryBarriers = &barrier};

    cmd.pipelineBarrier2(depInfo);
}

void RenderGraph::beginRenderPass(const vk::raii::CommandBuffer& cmd, const RenderPassNode& pass,
                                  const vk::Extent2D extent)
{
    if (pass.outputs.empty())
        return;

    std::vector<vk::RenderingAttachmentInfo> colorAttachments;
    colorAttachments.reserve(pass.outputs.size());

    for (auto handle : pass.outputs)
    {
        auto& [image, view] = images.at(handle);
        auto& desc = resources.at(handle);

        vk::RenderingAttachmentInfo attachment{
            .imageView = view,
            .imageLayout = vk::ImageLayout::eColorAttachmentOptimal,
            .loadOp = vk::AttachmentLoadOp::eClear,
            .storeOp = vk::AttachmentStoreOp::eStore,
            .clearValue = vk::ClearValue{std::array{
                desc.clearColor.r, desc.clearColor.g, desc.clearColor.b, desc.clearColor.a
            }}
        };

        colorAttachments.push_back(attachment);
    }

    vk::RenderingAttachmentInfo depthAttachment{};
    bool hasDepth = false;
    for (auto handle : pass.outputs)
    {
        auto& desc = resources.at(handle);
        if (desc.usage & vk::ImageUsageFlagBits::eDepthStencilAttachment)
        {
            hasDepth = true;
            auto& [image, view] = images.at(handle);
            depthAttachment = vk::RenderingAttachmentInfo{
                .imageView = view,
                .imageLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal,
                .loadOp = vk::AttachmentLoadOp::eClear,
                .storeOp = vk::AttachmentStoreOp::eStore,
                .clearValue = vk::ClearValue{vk::ClearDepthStencilValue{1.0f, 0}}
            };
            break;
        }
    }

    vk::RenderingInfo renderInfo{
        .renderArea = vk::Rect2D{.offset = {0, 0}, .extent = extent},
        .layerCount = 1,
        .colorAttachmentCount = static_cast<uint32_t>(colorAttachments.size()),
        .pColorAttachments = colorAttachments.data(),
        .pDepthAttachment = hasDepth ? &depthAttachment : nullptr,
    };

    cmd.beginRendering(renderInfo);
}

void RenderGraph::endRenderPass(const vk::raii::CommandBuffer& cmd) { cmd.endRendering(); }

void RenderGraph::analyzeLifetimes()
{
    for (uint32_t i = 0; i < passes.size(); ++i)
    {
        auto& p = *passes[i];
        for (auto h : p.inputs)
        {
            lifetimes[h].firstUse = std::min(lifetimes[h].firstUse, i);
            lifetimes[h].lastUse = std::max(lifetimes[h].lastUse, i);
        }
        for (auto h : p.outputs)
        {
            lifetimes[h].firstUse = std::min(lifetimes[h].firstUse, i);
            lifetimes[h].lastUse = std::max(lifetimes[h].lastUse, i);
        }
    }
}
} // namespace Nimbus::RenderGraph

#include "RenderGraph.h"

#include <iostream>
#include <ranges>

#include "RenderPassNode.h"

namespace Nimbus::RenderGraph
{
RenderGraph::RenderGraph() = default;
RenderGraph::~RenderGraph() = default;

void RenderGraph::allocateResources(vk::raii::Device& device, vk::Extent2D defaultExtent)
{
    struct AllocInfo
    {
        ResourceHandle handle;
        ResourceDesc desc;
        ResourceLifetime lifetime;
    };

    std::vector<AllocInfo> allocList;
    for (auto& [h, d] : resources)
        allocList.push_back({h, d, lifetimes[h]});

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
            .extent = {desc.extent.width ? desc.extent.width : defaultExtent.width,
                       desc.extent.height ? desc.extent.height : defaultExtent.height, 1},
            .mipLevels = 1,
            .arrayLayers = 1,
            .format = desc.format,
            .tiling = vk::ImageTiling::eOptimal,
            .initialLayout = vk::ImageLayout::eUndefined,
            .usage = desc.usage,
            .samples = vk::SampleCountFlagBits::e1,
        };

        vk::raii::Image image{device, info};

        vk::ImageViewCreateInfo viewInfo{.image = *image,
                                         .viewType = vk::ImageViewType::e2D,
                                         .format = desc.format,
                                         .subresourceRange = {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}};
        vk::raii::ImageView view{device, viewInfo};

        images[handle] = {std::move(image), std::move(view)};
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
                std::cerr << "[RenderGraph][Warning] Resource " << name
                          << " incompatible, creating new variant.\n";
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

void RenderGraph::compile()
{
    currentPassIndex = 0;
    for (const auto& pass : passes)
    {
        pass->setup(*this);
        ++currentPassIndex;
    }
    analyzeLifetimes();
}

void RenderGraph::execute()
{

}

void RenderGraph::beginRenderPass()
{

}

void RenderGraph::endRenderPass() {}

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

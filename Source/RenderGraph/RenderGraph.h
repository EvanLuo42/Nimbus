#pragma once
#include <unordered_map>
#include <unordered_set>

#include <vk_mem_alloc.h>
#include "RenderPassNode.h"
#include "RenderResource.h"

namespace Nimbus::RenderGraph
{
class RenderPassNode;

class RenderGraph
{
public:
    RenderGraph();
    explicit RenderGraph(const VmaAllocator allocator) : allocator(allocator) {}
    ~RenderGraph();

    template <typename T, typename... Args>
    T& addPass(Args&&... args)
    {
        static_assert(std::is_base_of_v<RenderPassNode, T>);
        auto node = std::make_unique<T>(std::forward<Args>(args)...);
        T& ref = *node;
        passes.emplace_back(std::move(node));
        return ref;
    }

    void allocateResources(vk::raii::Device& device, vk::Extent2D defaultExtent);

    ResourceHandle getOrCreateTexture(const std::string& name, const ResourceDesc& desc);

    void setup();
    void compile(vk::raii::Device& device);
    void execute(vk::raii::CommandBuffer& cmd, vk::Extent2D extent);

    std::vector<size_t> topologicalSort() const;

    void transitionImage(const vk::raii::CommandBuffer& cmd, ResourceHandle handle, vk::ImageLayout oldLayout,
                         vk::ImageLayout newLayout, vk::PipelineStageFlags2 srcStage, vk::PipelineStageFlags2 dstStage,
                         vk::AccessFlags2 srcAccess, vk::AccessFlags2 dstAccess);

    void analyzeLifetimes();

    struct ImageResource
    {
        vk::raii::Image image{nullptr};
        vk::raii::ImageView view{nullptr};
    };

    struct ResourceLifetime
    {
        uint32_t firstUse = std::numeric_limits<uint32_t>::max();
        uint32_t lastUse = 0;
    };

private:
    void beginRenderPass(const vk::raii::CommandBuffer& cmd, const RenderPassNode& pass, vk::Extent2D extent);
    void endRenderPass(const vk::raii::CommandBuffer& cmd);

    uint32_t nextId = 1;
    uint32_t currentPassIndex = 0;
    std::vector<std::unique_ptr<RenderPassNode>> passes;
    std::unordered_map<ResourceHandle, ResourceDesc> resources;
    std::unordered_map<ResourceHandle, ResourceLifetime> lifetimes;
    std::unordered_map<ResourceHandle, ImageResource> images;
    std::unordered_map<ResourceHandle, VmaAllocation> allocations;
    VmaAllocator allocator;
};
} // namespace Nimbus::RenderGraph

#pragma once
#include <unordered_map>

#include "RenderResource.h"

namespace Nimbus::RenderGraph
{
class RenderPassNode;

class RenderGraph {
public:
    RenderGraph();
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

    void compile();
    void execute();

    void analyzeLifetimes();

    struct ImageResource {
        vk::raii::Image image{nullptr};
        vk::raii::ImageView view{nullptr};
    };

    struct ResourceLifetime {
        uint32_t firstUse = std::numeric_limits<uint32_t>::max();
        uint32_t lastUse = 0;
    };

private:
    void beginRenderPass();
    void endRenderPass();

    uint32_t nextId = 1;
    uint32_t currentPassIndex = 0;
    std::vector<std::unique_ptr<RenderPassNode>> passes;
    std::unordered_map<ResourceHandle, ResourceDesc> resources;
    std::unordered_map<ResourceHandle, ResourceLifetime> lifetimes;
    std::unordered_map<ResourceHandle, ImageResource> images;
};
}
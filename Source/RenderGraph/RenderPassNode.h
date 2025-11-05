#pragma once
#include <string>

#include "RenderResource.h"

namespace Nimbus::RenderGraph
{
class RenderGraph;

class RenderPassNode
{
public:
    std::string name;
    std::vector<ResourceHandle> inputs;
    std::vector<ResourceHandle> outputs;

    explicit RenderPassNode(std::string n) : name(std::move(n)) {}
    virtual ~RenderPassNode() = default;

    virtual void setup(RenderGraph& graph) = 0;

    virtual void execute(vk::raii::CommandBuffer& cmd) = 0;
protected:
    ResourceHandle inputTexture(RenderGraph& graph, const std::string& name, const ResourceDesc& desc);

    ResourceHandle outputTexture(RenderGraph& graph, const std::string& name, const ResourceDesc& desc);
};
}

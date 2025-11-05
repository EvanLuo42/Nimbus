#include "RenderPassNode.h"
#include "RenderGraph.h"

namespace Nimbus::RenderGraph
{
ResourceHandle RenderPassNode::inputTexture(RenderGraph& graph,
                                            const std::string& name,
                                            const ResourceDesc& desc)
{
    const auto handle = graph.getOrCreateTexture(name, desc);
    inputs.push_back(handle);
    return handle;
}

ResourceHandle RenderPassNode::outputTexture(RenderGraph& graph,
                                             const std::string& name,
                                             const ResourceDesc& desc)
{
    const auto handle = graph.getOrCreateTexture(name, desc);
    outputs.push_back(handle);
    return handle;
}
}

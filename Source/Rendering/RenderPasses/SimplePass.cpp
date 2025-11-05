#include "SimplePass.h"

namespace Nimbus::Rendering::RenderPasses
{
void SimplePass::setup(RenderGraph::RenderGraph& graph)
{
    inputTexture(graph, "SimplePass.Color", {
        .format = vk::Format::eR8G8B8A8Unorm,
        .usage = vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eInputAttachment,
        .name = "SimplePass.Color"
    });
}

void SimplePass::execute(vk::raii::CommandBuffer& cmd)
{

}
}

#include "SimplePass.h"

namespace Nimbus::Rendering::RenderPasses
{
void SimplePass::setup(RenderGraph::RenderGraph& graph)
{
    inputTexture(graph, "SimplePass.ColorInput",
                 {.format = vk::Format::eR8G8B8A8Unorm,
                  .usage = vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eInputAttachment,
                  .name = "SimplePass.ColorInput"});

    output = outputTexture(graph, "SimplePass.ColorOutput",
                           {.format = vk::Format::eR8G8B8A8Unorm,
                            .usage = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled,
                            .name = "SimplePass.ColorOutput",
                            .clearColor = {1.0f, 0.0f, 0.0f, 1.0f}});
}

void SimplePass::execute(vk::raii::CommandBuffer& cmd)
{
    cmd.setViewport(
        0, vk::Viewport{.x = 0.0f, .y = 0.0f, .width = 1280.0f, .height = 720.0f, .minDepth = 0.0f, .maxDepth = 1.0f});

    cmd.setScissor(0, vk::Rect2D{{0, 0}, {1280, 720}});

    vk::ClearAttachment clearAttachment{
        .aspectMask = vk::ImageAspectFlagBits::eColor,
        .colorAttachment = 0,
        .clearValue = vk::ClearValue{ std::array<float, 4>{1.f, 0.f, 0.f, 1.f} }
    };

    vk::ClearRect clearRect{
        .rect = vk::Rect2D{{0, 0}, {1280, 720}},
        .baseArrayLayer = 0,
        .layerCount = 1
    };

    cmd.clearAttachments(clearAttachment, clearRect);
}
} // namespace Nimbus::Rendering::RenderPasses

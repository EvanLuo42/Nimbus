#include "Renderer.h"

#include "RenderPasses/SimplePass.h"

void Nimbus::Rendering::Renderer::render()
{
    beginFrame();
    device.waitIdle();
    const vk::CommandBufferAllocateInfo allocInfo{
        .commandPool = *commandPool, .level = vk::CommandBufferLevel::ePrimary, .commandBufferCount = 1 };

    auto commandBuffers = device.allocateCommandBuffers(allocInfo);
    vk::raii::CommandBuffer cmd{ std::move(commandBuffers[0]) };
    const vk::CommandBufferBeginInfo beginInfo{ .flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit };

    cmd.begin(beginInfo);

    renderGraph->execute(cmd, swapchainExtent);

    cmd.end();

    vk::SubmitInfo submitInfo{
        .commandBufferCount = 1,
        .pCommandBuffers = &*cmd
    };

    graphicsQueue.submit(submitInfo);
    graphicsQueue.waitIdle();
    endFrame();
}

void Nimbus::Rendering::Renderer::buildGraph()
{
    renderGraph->addPass<RenderPasses::SimplePass>("SimplePass");

    renderGraph->setup();
    renderGraph->compile(device);
}

void Nimbus::Rendering::Renderer::beginFrame()
{
    
}
void Nimbus::Rendering::Renderer::endFrame() {}

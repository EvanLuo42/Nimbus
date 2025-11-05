#include "Renderer.h"

#include "RenderPasses/SimplePass.h"

void Nimbus::Rendering::Renderer::render()
{
    beginFrame();
    renderGraph.compile();
    renderGraph.execute();
    endFrame();
}

void Nimbus::Rendering::Renderer::buildGraph()
{
    renderGraph.addPass<RenderPasses::SimplePass>("SimplePass");

    renderGraph.compile();
    renderGraph.allocateResources(device, vk::Extent2D{1280, 720});
}
void Nimbus::Rendering::Renderer::beginFrame() {}
void Nimbus::Rendering::Renderer::endFrame() {}

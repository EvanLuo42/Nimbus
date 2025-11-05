#include "Renderer.h"

void Nimbus::Rendering::Renderer::render()
{
    beginFrame();
    renderGraph.compile();
    renderGraph.execute();
    endFrame();
}

void Nimbus::Rendering::Renderer::buildGraph() {}
void Nimbus::Rendering::Renderer::beginFrame() {}
void Nimbus::Rendering::Renderer::endFrame() {}

#include "Renderer.h"

void Nimbus::Rendering::Renderer::render()
{
    renderGraph.compile();
    renderGraph.execute();
}

void Nimbus::Rendering::Renderer::buildGraph()
{

}

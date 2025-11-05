#pragma once
#include "../../RenderGraph/RenderPassNode.h"

namespace Nimbus::Rendering::RenderPasses
{
class SimplePass final : public RenderGraph::RenderPassNode {
public:
    explicit SimplePass(const std::string& n) : RenderPassNode{n} {}

    void setup(RenderGraph::RenderGraph& graph) override;
    void execute(vk::raii::CommandBuffer& cmd) override;

    RenderGraph::ResourceHandle color = UINT_MAX;
};
}
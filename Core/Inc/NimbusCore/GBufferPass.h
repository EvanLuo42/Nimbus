#pragma once

#include "NimbusRenderGraph/RenderPass.h"

namespace Nimbus::Core
{
class GBufferPass : public RenderGraph::RenderPass
{
public:
    void Setup() override;
    void Execute(CommandList& cmd) override;
};
}

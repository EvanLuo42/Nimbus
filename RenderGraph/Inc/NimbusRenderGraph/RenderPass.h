#pragma once


class CommandList;

namespace Nimbus::RenderGraph
{
struct RenderPass
{
    virtual void Setup() = 0;
    virtual void Execute(CommandList& cmd) = 0;
    virtual ~RenderPass() = default;

    RenderPassBuilder
};
} // namespace Nimbus::RenderGraph

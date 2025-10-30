#pragma once

#include <memory>
#include <vector>
#include "RenderPass.h"

namespace Nimbus::RenderGraph
{
class RenderGraph
{
public:
    template <typename T, typename... Args>
    T* AddRenderPass(Args&&... args);

private:
    std::vector<std::unique_ptr<RenderPass>> m_renderPasses;
};
} // namespace Nimbus::RenderGraph

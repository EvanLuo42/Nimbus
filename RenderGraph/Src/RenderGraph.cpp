#include "NimbusRenderGraph/RenderGraph.h"

template <typename T, typename... Args>
T* Nimbus::RenderGraph::RenderGraph::AddRenderPass(Args&&... args)
{
    static_assert(std::is_base_of_v<RenderPass, T>,
        "T must derive from RenderPass");

    auto pass = std::make_unique<T>(std::forward<Args>(args)...);
    T* passPtr = pass.get();

    m_renderPasses.emplace_back(std::move(pass));
    return passPtr;
}


#pragma once
#include <string>

namespace Nimbus::RenderGraph
{
class RenderPassBuilder
{
public:
    PassBuilder(ResourceSystem& rs) : m_resources(rs)
    {
    }

    TextureHandle CreateTexture(const std::string& name, const TextureDesc& desc)
    {
        return m_resources.Create(name, desc);
    }

    TextureHandle ReadTexture(const std::string& name)
    {
        return 0;
    }

private:
    ResourceSystem& m_resources;
};

} // namespace Nimbus::RenderGraph

#pragma once
#include <cstdint>
#include <memory>
#include <ranges>
#include <unordered_map>

#include "../../../RHI/Inc/NimbusRHI/Device.h"

namespace Nimbus::RHI
{
class Device;
class Texture;
struct TextureDesc;
} // namespace Nimbus::RHI

namespace Nimbus::RenderGraph
{
enum class ResourceType
{
    Texture,
    Buffer
};

template <ResourceType RT>
struct ResourceTrait;

template <>
struct ResourceTrait<ResourceType::Texture>
{
    using ResourceDesc = RHI::TextureDesc;
    using Resource = RHI::Texture;
};

struct TextureRef
{
    uint32_t m_id = UINT32_MAX;

    [[nodiscard]] bool IsValid() const
    {
        return m_id != UINT32_MAX;
    }
    bool operator==(const TextureRef& other) const
    {
        return m_id == other.m_id;
    }
};

template <ResourceType ResourceType>
struct Resource
{
    using Resource = ResourceTrait<ResourceType>::Resource;
    using ResourceDesc = ResourceTrait<ResourceType>::ResourceDesc;

    explicit Resource(std::shared_ptr<Resource> resource) : m_resource(std::move(resource)), m_desc(resource->GetDesc())
    {
    }

    explicit Resource(const ResourceDesc& desc) : m_resource(nullptr), m_desc(desc)
    {
    }

    std::shared_ptr<Resource> m_resource;
    ResourceDesc m_desc;
};

using TextureResource = Resource<ResourceType::Texture>;

class ResourceRegistry
{
public:
    TextureRef RegisterTexture(const RHI::TextureDesc& desc)
    {
        const TextureRef ref{m_nextId++};
        m_textures[ref.m_id] = TextureResource(desc);
        return ref;
    }

    [[nodiscard]] RHI::Texture* GetTexture(const TextureRef ref) const
    {
        return m_textures.at(ref.m_id).m_resource.get();
    }

    void AllocateTextures(RHI::Device* device)
    {
        for (auto& tex : m_textures | std::views::values)
            tex.m_resource = device->CreateTexture(tex.m_desc);
    }

private:
    uint32_t m_nextId = 0;
    std::unordered_map<uint32_t, TextureResource> m_textures;
};
} // namespace Nimbus::RenderGraph

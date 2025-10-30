#pragma once
#include <cstdint>

#include "DescriptorHandle.h"
#include "Resource.h"

namespace Nimbus::RHI
{
enum class TextureFormat
{
    Unknown,
    RGBA8_UNorm,
    RGBA16_Float,
    Depth24_Stencil8,
};

enum class TextureUsage : uint32_t
{
    ShaderResource = 1 << 0,
    RenderTarget = 1 << 1,
    DepthStencil = 1 << 2,
    Storage = 1 << 3,
    CopySrc = 1 << 4,
    CopyDst = 1 << 5
};

struct TextureDesc
{
    uint32_t m_width = 1;
    uint32_t m_height = 1;
    uint32_t m_depth = 1;
    uint32_t m_mipLevels = 1;
    TextureFormat m_format = TextureFormat::RGBA8_UNorm;
    TextureUsage m_usage = TextureUsage::ShaderResource;
    const char* m_debugName = nullptr;
};

class Texture : public Resource
{
public:
    explicit Texture(const TextureDesc& desc) : m_desc(desc)
    {
    }
    virtual ~Texture() = default;

    [[nodiscard]] const TextureDesc& GetDesc() const
    {
        return m_desc;
    }

    [[nodiscard]] virtual DescriptorHandle GetSRV() const = 0;
    [[nodiscard]] virtual DescriptorHandle GetUAV(uint32_t mip = 0) const = 0;
    [[nodiscard]] virtual DescriptorHandle GetRTV(uint32_t mip = 0) const = 0;
    [[nodiscard]] virtual DescriptorHandle GetDSV() const = 0;

protected:
    TextureDesc m_desc;
};
} // namespace Nimbus::RHI

#pragma once
#include <memory>


#include "Buffer.h"
#include "Texture.h"

namespace Nimbus::RHI
{
class Device
{
public:
    virtual ~Device() = default;
    virtual std::shared_ptr<Texture> CreateTexture(const TextureDesc& desc) = 0;
    virtual std::shared_ptr<Buffer> CreateBuffer(const BufferDesc& desc) = 0;
    virtual void Present() = 0;
};
} // namespace Nimbus::RHI

#pragma once
#include <cstdint>
#include "Resource.h"

namespace Nimbus::RHI
{
    enum class BufferUsage : uint32_t
    {
        Vertex = 1 << 0,
        Index = 1 << 1,
        Constant = 1 << 2,
        Storage = 1 << 3,
        CopySrc = 1 << 4,
        CopyDst = 1 << 5
    };

    struct BufferDesc
    {
        uint64_t m_size = 0;
        BufferUsage m_usage = BufferUsage::Vertex;
        const char* m_debugName = nullptr;
    };

    class Buffer : public Resource
    {
    public:
        explicit Buffer(const BufferDesc& desc) : m_desc(desc) {}
        virtual ~Buffer() = default;

        [[nodiscard]] const BufferDesc& GetDesc() const { return m_desc; }

        virtual void* Map() = 0;
        virtual void Unmap() = 0;
        virtual void Upload(const void* data, size_t size, size_t offset = 0) = 0;

    protected:
        BufferDesc m_desc;
    };
}
#pragma once
#include <cstdint>

namespace Nimbus::RHI
{
struct DescriptorHandle
{
    uint64_t m_cpuHandle = 0;
    uint64_t m_gpuHandle = 0;
    [[nodiscard]] bool IsValid() const
    {
        return m_cpuHandle != 0;
    }
};
} // namespace Nimbus::RHI

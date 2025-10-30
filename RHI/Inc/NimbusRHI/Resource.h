#pragma once
#include <string>

namespace Nimbus::RHI
{
class Resource
{
public:
    virtual ~Resource() = default;

    [[nodiscard]] virtual const char* GetName() const = 0;
    [[nodiscard]] virtual uint64_t GetSize() const = 0;

    virtual void TransitionToState(uint32_t newState) = 0;
};
} // namespace Nimbus::RHI

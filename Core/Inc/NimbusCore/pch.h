#pragma once

#include <cstdio>
#include <exception>
#include <intsafe.h>

namespace Nimbus::Core
{
class ComException : public std::exception
{
public:
    explicit ComException(const HRESULT hr) : m_result(hr)
    {
    }

    virtual const char* what() const override
    {
        static char sStr[64] = {};
        sprintf_s(sStr, "Failure with HRESULT of %08X", static_cast<unsigned int>(m_result));
        return sStr;
    }

private:
    HRESULT m_result;
};

inline void ThrowIfFailed(const HRESULT hr)
{
    if (FAILED(hr))
    {
        throw ComException(hr);
    }
}
} // namespace Nimbus::Core

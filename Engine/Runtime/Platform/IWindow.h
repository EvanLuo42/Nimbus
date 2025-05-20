#pragma once

#include <string>

struct NativeWindowHandle
{
    enum class Type
    {
        Win32,
        Cocoa
    };

    Type Type;
    void *Handle;
    void *Extra;
};

class IWindow {
public:
    virtual ~IWindow() = default;

    virtual void PollEvents() = 0;
    [[nodiscard]] virtual bool ShouldClose() const = 0;
    [[nodiscard]] virtual NativeWindowHandle GetNativeHandle() const = 0;
    virtual void SetTitle(const std::string& title) = 0;
};

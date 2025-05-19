#pragma once
#include <cstdint>
#include <memory>

struct NativeWindowHandle {
    enum class Type {
        Glfw,
        Sdl,
        Cocoa,
        Win32,
        X11
    };

    Type Type;
    void* Handle = nullptr;
};

class IWindow {
public:
    virtual ~IWindow() = default;
    virtual void PollEvents() = 0;
    [[nodiscard]] virtual NativeWindowHandle GetNativeHandle() const = 0;
    [[nodiscard]] virtual bool ShouldClose() const = 0;
    virtual void SwapBuffers() = 0;

    virtual uint32_t GetWidth();
    virtual uint32_t GetHeight();
};

#pragma once

#include <GLFW/glfw3.h>
#include "IWindow.h"

class GlfwWindow final : public IWindow {
    GLFWwindow *Window = nullptr;
    int Width, Height;
public:
    explicit GlfwWindow(int width, int height, const char* title);
    ~GlfwWindow() override;

    void PollEvents() override;
    [[nodiscard]] NativeWindowHandle GetNativeHandle() const override;
    [[nodiscard]] bool ShouldClose() const override;
    void SwapBuffers() override;

    uint32_t GetWidth() override;
    uint32_t GetHeight() override;
};

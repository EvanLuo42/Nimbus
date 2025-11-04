#pragma once
#include <string>


#include "GLFW/glfw3.h"

namespace Nimbus::Platform
{
class Window
{
public:
    struct WindowDesc
    {
        int width = 1280;
        int height = 720;
        std::string title = "Nimbus Renderer";
    };

    explicit Window(const WindowDesc& desc);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(Window&&) = delete;

    static void pollEvents();
    [[nodiscard]] bool shouldClose() const;
    [[nodiscard]] void* getNativeHandle() const { return window; }

    [[nodiscard]] int getWidth() const { return width; }
    [[nodiscard]] int getHeight() const { return height; }

private:
    GLFWwindow* window = nullptr;
    int width = 0;
    int height = 0;
};
} // namespace Nimbus::Platform

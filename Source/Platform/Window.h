#pragma once
#include <string>

#define GLFW_INCLUDE_VULKAN
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

    static void pollEvents();
    [[nodiscard]] bool shouldClose() const;
    [[nodiscard]] GLFWwindow* getNativeHandle() const { return window; }

    [[nodiscard]] int getWidth() const { return width; }
    [[nodiscard]] int getHeight() const { return height; }
    [[nodiscard]] const std::string& getTitle() const { return title; }

private:
    GLFWwindow* window = nullptr;
    int width = 0;
    int height = 0;
    std::string title;
};
} // namespace Nimbus::Platform

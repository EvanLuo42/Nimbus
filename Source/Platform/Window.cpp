#include "Window.h"

#include <stdexcept>


Nimbus::Platform::Window::Window(const WindowDesc& desc)
{
    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window = glfwCreateWindow(desc.width, desc.height, desc.title.c_str(), nullptr, nullptr);

    if (!window)
        throw std::runtime_error("Failed to create GLFW window");

    width = desc.width;
    height = desc.height;
    title = desc.title;
}

Nimbus::Platform::Window::~Window()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Nimbus::Platform::Window::pollEvents()
{
    glfwPollEvents();
}

bool Nimbus::Platform::Window::shouldClose() const
{
    return glfwWindowShouldClose(window);
}

#include "GlfwWindow.h"

#include <GLFW/glfw3.h>
#include <stdexcept>

GlfwWindow::GlfwWindow(const int width, const int height, const char *title) : Width{width}, Height{height} {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    Window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!Window) {
        throw std::runtime_error("Failed to create GLFW window");
    }
}

GlfwWindow::~GlfwWindow() {
    glfwDestroyWindow(Window);
    glfwTerminate();
}

void GlfwWindow::PollEvents() {
    glfwPollEvents();
}

NativeWindowHandle GlfwWindow::GetNativeHandle() const {
    return NativeWindowHandle {
        .Type = NativeWindowHandle::Type::Glfw,
        .Handle = static_cast<void*>(Window)
    };
}

bool GlfwWindow::ShouldClose() const {
    return glfwWindowShouldClose(Window);
}

void GlfwWindow::SwapBuffers() { glfwSwapBuffers(Window); }

uint32_t GlfwWindow::GetWidth() {
    return Width;
}

uint32_t GlfwWindow::GetHeight() {
    return Height;
}


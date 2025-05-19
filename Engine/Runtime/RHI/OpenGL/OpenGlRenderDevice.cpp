#include "OpenGlRenderDevice.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stdexcept>

void OpenGlRenderDevice::InitWithWindow(const NativeWindowHandle &window) {
    switch (window.Type) {
        case NativeWindowHandle::Type::Glfw:
            glfwMakeContextCurrent(static_cast<GLFWwindow *>(window.Handle));
            gladLoadGL();
            break;
        default:
            throw std::runtime_error("Unsupported window type for OpenGL backend");
    }
}

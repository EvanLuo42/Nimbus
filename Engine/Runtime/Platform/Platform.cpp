#include "Platform.h"

#include "Input/GlfwInput.h"
#include "Window/GlfwWindow.h"
#include <GLFW/glfw3.h>

PlatformContext::PlatformContext(std::unique_ptr<IWindow> window, std::unique_ptr<::Input> input) :
    Input{std::move(input)}, Window{std::move(window)} {}

IWindow *PlatformContext::GetWindow() const {
    return Window.get();
}

Input *PlatformContext::GetInput() const {
    return Input.get();
}

PlatformContext CreatePlatformContext(int width, int height, const char* title) {
    auto window = std::make_unique<GlfwWindow>(width, height, title);
    auto input = std::make_unique<GlfwInput>(static_cast<GLFWwindow*>(window->GetNativeHandle()));
    PlatformContext platform(std::move(window), std::move(input));
    return platform;
}

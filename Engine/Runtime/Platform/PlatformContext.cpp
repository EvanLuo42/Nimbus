#include "PlatformContext.h"

PlatformContext::PlatformContext(std::unique_ptr<IWindow> window, std::unique_ptr<IInput> input) :
    Window{std::move(window)}, Input{std::move(input)} {}

IWindow *PlatformContext::GetWindow() const
{
    return Window.get();
}

IInput *PlatformContext::GetInput() const
{
    return Input.get();
}


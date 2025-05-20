#include "MacOSWindow.h"

#include "Include/MacOSPlatform.h"

void MacOsWindow::PollEvents()
{
    MacOSPlatform::Program::pollEvents();
}

bool MacOsWindow::ShouldClose() const
{
    return MacOSPlatform::Program::shouldCloseFlag();
}

NativeWindowHandle MacOsWindow::GetNativeHandle() const
{
    return NativeWindowHandle {
        NativeWindowHandle::Type::Cocoa,
        Window,
        nullptr
    };
}

void MacOsWindow::SetTitle(const std::string &title)
{
    MacOSPlatform::Program::setTitle(swift::String(title));
}
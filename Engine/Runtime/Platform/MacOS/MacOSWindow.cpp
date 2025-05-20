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

void *MacOsWindow::GetNativeHandle() const
{
    return Window;
}

void MacOsWindow::SetTitle(const std::string &title)
{
    MacOSPlatform::Program::setTitle(swift::String(title));
}
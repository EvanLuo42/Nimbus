#pragma once
#include "Core/Utils/LogUtils.h"
#include "Include/MacOSPlatform.h"
#include "Platform/IWindow.h"
#include "spdlog/spdlog.h"

class MacOsWindow final : public IWindow
{
  public:
    MacOsWindow(const int width, const int height, const char *title)
    {
        Window = MacOSPlatform::Program::createWindow(width, height, title);
        NB_CORE_DEBUG("Create a macOS window: Width: {}, Height: {}, Title: {}", width, height, title);
    }
    void PollEvents() override;
    [[nodiscard]] bool ShouldClose() const override;
    [[nodiscard]] NativeWindowHandle GetNativeHandle() const override;
    void SetTitle(const std::string &title) override;

  private:
    void* Window;
};

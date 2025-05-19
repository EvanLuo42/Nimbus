#pragma once
#include "IInput.h"
#include "IWindow.h"

class PlatformContext {
    int Width = 0;
    int Height = 0;
    const char *Title = "";
public:
    static PlatformContext* CreatePlatform(int width, int height, const char *title);

    virtual IWindow* GetWindow() = 0;
    virtual IInput* GetInput() = 0;
    virtual ~PlatformContext() = default;
};

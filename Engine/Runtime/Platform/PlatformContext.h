#pragma once
#include "IInput.h"
#include "IWindow.h"

#include <memory>

class PlatformContext {
    int Width = 0;
    int Height = 0;
    const char *Title = "";

    std::unique_ptr<IWindow> Window;
    std::unique_ptr<IInput> Input;
public:
    PlatformContext(std::unique_ptr<IWindow> window, std::unique_ptr<IInput> input);

    [[nodiscard]] IWindow* GetWindow() const;
    [[nodiscard]] IInput* GetInput() const;
    ~PlatformContext() = default;
};

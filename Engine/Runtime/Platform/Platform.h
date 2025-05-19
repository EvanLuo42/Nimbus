#pragma once
#include <memory>

#include "Input/Input.h"
#include "Window/IWindow.h"

class PlatformContext {
public:
    PlatformContext(std::unique_ptr<IWindow> window, std::unique_ptr<Input> input);

    [[nodiscard]] IWindow* GetWindow() const;
    [[nodiscard]] Input* GetInput() const;

private:
    std::unique_ptr<Input> Input;
    std::unique_ptr<IWindow> Window;
};
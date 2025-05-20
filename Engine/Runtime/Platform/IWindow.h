#pragma once

#include <string>

class IWindow {
public:
    virtual ~IWindow() = default;

    virtual void PollEvents() = 0;
    [[nodiscard]] virtual bool ShouldClose() const = 0;
    [[nodiscard]] virtual void* GetNativeHandle() const = 0;
    virtual void SetTitle(const std::string& title) = 0;
};
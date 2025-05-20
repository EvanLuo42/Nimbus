#pragma once
#include "Platform/IInput.h"

#include <tuple>

class MacOsInput final : IInput {
public:
    [[nodiscard]] bool IsKeyPressed(KeyCode key) const override;
    [[nodiscard]] bool IsMouseButtonPressed(MouseButton button) const override;
    [[nodiscard]] std::tuple<double, double> GetMousePosition() const override;
};

#pragma once

enum class KeyCode {
    W, A, S, D, Escape,
};

class IInput {
public:
    virtual ~IInput() = default;
    [[nodiscard]] virtual bool IsKeyPressed(KeyCode key) const = 0;
    [[nodiscard]] virtual bool IsMouseButtonPressed(int button) const = 0;
    virtual void GetMousePosition(double& x, double& y) const = 0;
};
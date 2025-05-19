#pragma once

#include "Input.h"
#include "Platform/Window/GlfwWindow.h"

class GlfwInput final : public Input {
    GLFWwindow *Window = nullptr;
public:
    explicit GlfwInput(GLFWwindow *window);
    ~GlfwInput() override;

    void Init() override;
    bool IsKeyPressed(Key key) override;
    bool IsMouseButtonPressed(Button button) override;
    double GetMouseX() override;
    double GetMouseY() override;
};

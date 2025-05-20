#include "MacOSInput.h"

#include "Include/MacOSPlatform.h"

bool MacOsInput::IsKeyPressed(KeyCode key) const
{
    return MacOSPlatform::Program::isKeyPressed(static_cast<uint16_t>(key));
}

bool MacOsInput::IsMouseButtonPressed(const MouseButton button) const
{
    switch (button)
    {
    case MouseButton::Left:
        return MacOSPlatform::Program::isLeftClicked();
    case MouseButton::Right:
        return MacOSPlatform::Program::isRightClicked();
    case MouseButton::Middle:
        return MacOSPlatform::Program::isMiddleClicked();
    }
    return false;
}

std::tuple<double, double> MacOsInput::GetMousePosition() const
{
    const auto position = MacOSPlatform::Program::getMousePosition();
    return std::make_tuple(position[0], position[1]);
}
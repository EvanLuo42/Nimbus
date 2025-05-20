#pragma once
#include <cstdint>
#include <tuple>

enum class KeyCode : uint16_t {
    Zero = 29,
    One = 18,
    Two = 19,
    Three = 20,
    Four = 21,
    Five = 23,
    Six = 22,
    Seven = 26,
    Eight = 28,
    Nine = 25,

    A = 0,
    B = 11,
    C = 8,
    D = 2,
    E = 14,
    F = 3,
    G = 5,
    H = 4,
    I = 34,
    J = 38,
    K = 40,
    L = 37,
    M = 46,
    N = 45,
    O = 31,
    P = 35,
    Q = 12,
    R = 15,
    S = 1,
    T = 17,
    U = 32,
    V = 9,
    W = 13,
    X = 7,
    Y = 16,
    Z = 6,

    SectionSign = 10,
    Grave = 50,
    Minus = 27,
    Equal = 24,
    LeftBracket = 33,
    RightBracket = 30,
    Semicolon = 41,
    Quote = 39,
    Comma = 43,
    Period = 47,
    Slash = 44,
    Backslash = 42,

    Keypad0 = 82,
    Keypad1 = 83,
    Keypad2 = 84,
    Keypad3 = 85,
    Keypad4 = 86,
    Keypad5 = 87,
    Keypad6 = 88,
    Keypad7 = 89,
    Keypad8 = 91,
    Keypad9 = 92,
    KeypadDecimal = 65,
    KeypadMultiply = 67,
    KeypadPlus = 69,
    KeypadDivide = 75,
    KeypadMinus = 78,
    KeypadEquals = 81,
    KeypadClear = 71,
    KeypadEnter = 76,

    Space = 49,
    Return = 36,
    Tab = 48,
    Delete = 51,
    ForwardDelete = 117,
    Linefeed = 52,
    Escape = 53,

    Command = 55,
    Shift = 56,
    CapsLock = 57,
    Option = 58,
    Control = 59,
    RightShift = 60,
    RightOption = 61,
    RightControl = 62,
    RightCommand = 54,
    Function = 63,

    F1 = 122,
    F2 = 120,
    F3 = 99,
    F4 = 118,
    F5 = 96,
    F6 = 97,
    F7 = 98,
    F8 = 100,
    F9 = 101,
    F10 = 109,
    F11 = 103,
    F12 = 111,
    F13 = 105,
    F14 = 107,
    F15 = 113,
    F16 = 106,
    F17 = 64,
    F18 = 79,
    F19 = 80,
    F20 = 90,

    VolumeUp = 72,
    VolumeDown = 73,
    Mute = 74,

    HelpInsert = 114,
    Home = 115,
    End = 119,
    PageUp = 116,
    PageDown = 121,

    LeftArrow = 123,
    RightArrow = 124,
    DownArrow = 125,
    UpArrow = 126
};

inline const char* ToString(const KeyCode code) {
    switch (code) {
        case KeyCode::Zero: return "Zero";
        case KeyCode::One: return "One";
        case KeyCode::Two: return "Two";
        case KeyCode::Three: return "Three";
        case KeyCode::Four: return "Four";
        case KeyCode::Five: return "Five";
        case KeyCode::Six: return "Six";
        case KeyCode::Seven: return "Seven";
        case KeyCode::Eight: return "Eight";
        case KeyCode::Nine: return "Nine";

        case KeyCode::A: return "A";
        case KeyCode::B: return "B";
        case KeyCode::C: return "C";
        case KeyCode::D: return "D";
        case KeyCode::E: return "E";
        case KeyCode::F: return "F";
        case KeyCode::G: return "G";
        case KeyCode::H: return "H";
        case KeyCode::I: return "I";
        case KeyCode::J: return "J";
        case KeyCode::K: return "K";
        case KeyCode::L: return "L";
        case KeyCode::M: return "M";
        case KeyCode::N: return "N";
        case KeyCode::O: return "O";
        case KeyCode::P: return "P";
        case KeyCode::Q: return "Q";
        case KeyCode::R: return "R";
        case KeyCode::S: return "S";
        case KeyCode::T: return "T";
        case KeyCode::U: return "U";
        case KeyCode::V: return "V";
        case KeyCode::W: return "W";
        case KeyCode::X: return "X";
        case KeyCode::Y: return "Y";
        case KeyCode::Z: return "Z";

        case KeyCode::SectionSign: return "SectionSign";
        case KeyCode::Grave: return "Grave";
        case KeyCode::Minus: return "Minus";
        case KeyCode::Equal: return "Equal";
        case KeyCode::LeftBracket: return "LeftBracket";
        case KeyCode::RightBracket: return "RightBracket";
        case KeyCode::Semicolon: return "Semicolon";
        case KeyCode::Quote: return "Quote";
        case KeyCode::Comma: return "Comma";
        case KeyCode::Period: return "Period";
        case KeyCode::Slash: return "Slash";
        case KeyCode::Backslash: return "Backslash";

        case KeyCode::Keypad0: return "Keypad0";
        case KeyCode::Keypad1: return "Keypad1";
        case KeyCode::Keypad2: return "Keypad2";
        case KeyCode::Keypad3: return "Keypad3";
        case KeyCode::Keypad4: return "Keypad4";
        case KeyCode::Keypad5: return "Keypad5";
        case KeyCode::Keypad6: return "Keypad6";
        case KeyCode::Keypad7: return "Keypad7";
        case KeyCode::Keypad8: return "Keypad8";
        case KeyCode::Keypad9: return "Keypad9";
        case KeyCode::KeypadDecimal: return "KeypadDecimal";
        case KeyCode::KeypadMultiply: return "KeypadMultiply";
        case KeyCode::KeypadPlus: return "KeypadPlus";
        case KeyCode::KeypadDivide: return "KeypadDivide";
        case KeyCode::KeypadMinus: return "KeypadMinus";
        case KeyCode::KeypadEquals: return "KeypadEquals";
        case KeyCode::KeypadClear: return "KeypadClear";
        case KeyCode::KeypadEnter: return "KeypadEnter";

        case KeyCode::Space: return "Space";
        case KeyCode::Return: return "Return";
        case KeyCode::Tab: return "Tab";
        case KeyCode::Delete: return "Delete";
        case KeyCode::ForwardDelete: return "ForwardDelete";
        case KeyCode::Linefeed: return "Linefeed";
        case KeyCode::Escape: return "Escape";

        case KeyCode::Command: return "Command";
        case KeyCode::Shift: return "Shift";
        case KeyCode::CapsLock: return "CapsLock";
        case KeyCode::Option: return "Option";
        case KeyCode::Control: return "Control";
        case KeyCode::RightShift: return "RightShift";
        case KeyCode::RightOption: return "RightOption";
        case KeyCode::RightControl: return "RightControl";
        case KeyCode::RightCommand: return "RightCommand";
        case KeyCode::Function: return "Function";

        case KeyCode::F1: return "F1";
        case KeyCode::F2: return "F2";
        case KeyCode::F3: return "F3";
        case KeyCode::F4: return "F4";
        case KeyCode::F5: return "F5";
        case KeyCode::F6: return "F6";
        case KeyCode::F7: return "F7";
        case KeyCode::F8: return "F8";
        case KeyCode::F9: return "F9";
        case KeyCode::F10: return "F10";
        case KeyCode::F11: return "F11";
        case KeyCode::F12: return "F12";
        case KeyCode::F13: return "F13";
        case KeyCode::F14: return "F14";
        case KeyCode::F15: return "F15";
        case KeyCode::F16: return "F16";
        case KeyCode::F17: return "F17";
        case KeyCode::F18: return "F18";
        case KeyCode::F19: return "F19";
        case KeyCode::F20: return "F20";

        case KeyCode::VolumeUp: return "VolumeUp";
        case KeyCode::VolumeDown: return "VolumeDown";
        case KeyCode::Mute: return "Mute";

        case KeyCode::HelpInsert: return "Help_Insert";
        case KeyCode::Home: return "Home";
        case KeyCode::End: return "End";
        case KeyCode::PageUp: return "PageUp";
        case KeyCode::PageDown: return "PageDown";

        case KeyCode::LeftArrow: return "LeftArrow";
        case KeyCode::RightArrow: return "RightArrow";
        case KeyCode::DownArrow: return "DownArrow";
        case KeyCode::UpArrow: return "UpArrow";

        default: return "Unknown";
    }
}

enum class KeyState
{
    Pressed,
    Released
};

inline const char* ToString(const KeyState state) {
    switch (state) {
        case KeyState::Pressed: return "Pressed";
        case KeyState::Released: return "Released";
        default: return "Unknown";
    }
}

enum class MouseButton : uint16_t
{
    Left = 0,
    Right = 1,
    Middle = 2
};

class IInput {
public:
    virtual ~IInput() = default;
    [[nodiscard]] virtual bool IsKeyPressed(KeyCode key) const = 0;
    [[nodiscard]] virtual bool IsMouseButtonPressed(MouseButton button) const = 0;
    [[nodiscard]] virtual std::tuple<double, double> GetMousePosition() const = 0;
};
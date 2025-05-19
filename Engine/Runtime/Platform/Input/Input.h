#pragma once

enum class Key {

};

enum class Button {

};

class Input {
public:
    virtual ~Input() = default;
    virtual void Init();
    virtual bool IsKeyPressed(Key key);
    virtual bool IsMouseButtonPressed(Button button);
    virtual double GetMouseX();
    virtual double GetMouseY();
};

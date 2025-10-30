#pragma once

#include <memory>
#include "Win32Window.h"

class Application
{
public:
    Application(HINSTANCE hInstance, uint32_t width, uint32_t height);
    void Run();

private:
    void OnUpdate();
    void OnRender() const;
    void OnDestroy();

    std::unique_ptr<Win32Window> m_window;

    bool m_running = true;
};

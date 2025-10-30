#pragma once

#include <memory>
#include "NimbusCore/Device.h"
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
    std::unique_ptr<Nimbus::Core::Device> m_device;

    bool m_running = true;
};

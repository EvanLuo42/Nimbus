#include "Application.h"
#include <chrono>

Application::Application(HINSTANCE hInstance, uint32_t width, uint32_t height)
{
    m_window = std::make_unique<Win32Window>(hInstance, width, height, L"Nimbus Renderer");
}

void Application::Run()
{
    while (true)
    {
        if (!m_window->ProcessMessages())
            break;

        if (!m_running)
            break;

        OnUpdate();
        OnRender();
    }
    OnDestroy();
}

void Application::OnUpdate()
{
}

void Application::OnRender() const
{
    
}

void Application::OnDestroy()
{
    FreeConsole();
    ExitProcess(0);
}

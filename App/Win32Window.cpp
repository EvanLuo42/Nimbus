#include "Win32Window.h"

Win32Window::Win32Window(const HINSTANCE hInstance, const uint32_t width, const uint32_t height, std::wstring title) :
    m_hInstance(hInstance), m_title(std::move(title)), m_width(width), m_height(height)
{
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"NimbusRenderer";
    RegisterClass(&wc);

    m_hwnd =
        CreateWindowEx(0, wc.lpszClassName, m_title.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                       static_cast<int>(m_width), static_cast<int>(m_height), nullptr, nullptr, hInstance, nullptr);

    ShowWindow(m_hwnd, SW_SHOW);
}

Win32Window::~Win32Window()
{
    DestroyWindow(m_hwnd);
}

bool Win32Window::ProcessMessages()
{
    MSG msg = {};
    while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
            return false;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true;
}

LRESULT CALLBACK Win32Window::WindowProc(const HWND hwnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

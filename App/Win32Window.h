#pragma once
#include <Windows.h>
#include <cstdint>
#include <string>

class Win32Window
{
public:
    Win32Window(HINSTANCE hInstance, uint32_t width, uint32_t height, std::wstring title);
    ~Win32Window();

    [[nodiscard]] HWND GetHandle() const { return m_hwnd; }
    bool ProcessMessages();

private:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    HWND m_hwnd = nullptr;
    HINSTANCE m_hInstance = nullptr;
    std::wstring m_title;
    uint32_t m_width = 0;
    uint32_t m_height = 0;
};

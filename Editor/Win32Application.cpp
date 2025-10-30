#include "Win32Application.h"

HWND DirectXRenderer::Editor::Win32Application::Hwnd = nullptr;

int DirectXRenderer::Editor::Win32Application::Run(Editor *editor, HINSTANCE hInstance, int nShowCmd)
{
    WNDCLASSEX windowClass{};
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = WindowProc;
    windowClass.hInstance = hInstance;
    windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    windowClass.lpszClassName = reinterpret_cast<LPCSTR>(L"DirectXRenderer");
    RegisterClassEx(&windowClass);

    RECT windowRect = {0, 0, static_cast<LONG>(editor->GetWidth()), static_cast<LONG>(editor->GetHeight())};
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    Hwnd = CreateWindow(windowClass.lpszClassName, reinterpret_cast<LPCSTR>(editor->GetTitle()), WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, CW_USEDEFAULT, windowRect.right - windowRect.left,
                        windowRect.bottom - windowRect.top, nullptr, nullptr, hInstance, editor);

    editor->OnInit();

    ShowWindow(Hwnd, nShowCmd);

    MSG msg = {};
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    editor->OnDestroy();

    return static_cast<char>(msg.wParam);
}

LRESULT CALLBACK DirectXRenderer::Editor::Win32Application::WindowProc(HWND hWnd, UINT message, WPARAM wParam,
                                                                       LPARAM lParam)
{
    auto editor = reinterpret_cast<Editor *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    switch (message)
    {
    case WM_CREATE: {
        const auto pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
    }
        return 0;

    case WM_PAINT:
        if (editor)
        {
            editor->OnUpdate();
            editor->OnRender();
        }
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}
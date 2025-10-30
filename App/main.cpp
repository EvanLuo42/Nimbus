#include <Windows.h>
#include <iostream>
#include <ostream>

#include "Application.h"

void CreateDebugConsole()
{
    AllocConsole();

    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
    freopen_s(&fp, "CONIN$", "r", stdin);

    std::ios::sync_with_stdio();
}

#ifdef _DEBUG
int main() {
    CreateDebugConsole();
    const HINSTANCE hInstance = GetModuleHandle(nullptr);
#else
_Use_decl_annotations_ int WINAPI WinMain(const HINSTANCE hInstance, HINSTANCE, LPSTR, int nShowCmd) {
#endif
    Application app(hInstance, 1280, 720);
    app.Run();
    return 0;
}
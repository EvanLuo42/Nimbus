#include <iostream>

#include "Application.h"

#ifdef _WIN32
#include <windows.h>

static void createDebugConsole()
{
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    freopen_s(&f, "CONOUT$", "w", stderr);
    freopen_s(&f, "CONIN$", "r", stdin);
    std::ios::sync_with_stdio();

    SetConsoleTitleA("Nimbus Debug Console");
}
#endif

int main(const int argc, char* argv[])
{
#ifdef _WIN32
    createDebugConsole();
#endif

    try
    {
        Application application{argc, argv};
        application.run();
    }
    catch (const Nimbus::Utils::Error::ArgParseError& argErr)
    {
        std::cerr << argErr.what();
        return 1;
    }

    return 0;
}

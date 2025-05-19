#include "Nimbus.h"

#include "Include/MacOSPlatform.h"

void Nimbus::CreateEngine() {
    MacOSPlatform::Program::createWindow(800, 600, swift::String("Nimbus Engine"));
    while (!MacOSPlatform::Program::shouldCloseFlag())
    {
        MacOSPlatform::Program::pollEvents();
    }
}

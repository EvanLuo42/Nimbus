#pragma once
#include "Platform/Window.h"
#include "Utils/ArgParser.h"

class Application {
public:
    Application(int argc, char* argv[]);
    ~Application() = default;

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(Application&&) = delete;

    void run() const;

private:
    Nimbus::Utils::ArgParser args;
    Nimbus::Platform::Window window;
};

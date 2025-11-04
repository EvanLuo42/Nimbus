#include "Application.h"

Application::Application(const int argc, char* argv[]) :
    args(argc, argv), window({args.getInt("width").value_or(1280), args.getInt("height").value_or(720),
                              args.getString("title").value_or("Nimbus Renderer")})
{
}

void Application::run() const
{
    while (!window.shouldClose())
    {
        window.pollEvents();
    }
}

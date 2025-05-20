#include "Nimbus.h"

#include "Event/Event.h"
#include "Event/ResizeEvent.h"
#include "Platform/MacOS/MacOSWindow.h"

#include <print>

void Nimbus::CreateEngine()
{
    EventBus::Instance().Get<ResizeEvent>().Register([](const ResizeEvent& e) {
        std::println("Width: {}, Height: {}", e.Width, e.Height);
    });

    auto macOsWindow = MacOsWindow{800, 600, "Nimbus Engine"};

    while (!macOsWindow.ShouldClose())
    {
        macOsWindow.PollEvents();
    }
}

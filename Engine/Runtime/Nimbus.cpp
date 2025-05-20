#include "Nimbus.h"

#include "Event/Event.h"
#include "Event/KeyEvent.h"
#include "Event/ResizeEvent.h"
#include "Platform/MacOS/MacOSWindow.h"

#include <print>

void Nimbus::CreateEngine()
{
    EventBus::Instance().Get<ResizeEvent>().Register([](const ResizeEvent& e) {
        std::println("Width: {}, Height: {}", e.Width, e.Height);
    });

    EventBus::Instance().Get<KeyEvent>().Register([](const auto &e) {
        std::println("Key code: {}, Key state: {}", ToString(e.Code), ToString(e.State));
    });

    auto macOsWindow = MacOsWindow{800, 600, "Nimbus Engine"};

    while (!macOsWindow.ShouldClose())
    {
        macOsWindow.PollEvents();
    }
}

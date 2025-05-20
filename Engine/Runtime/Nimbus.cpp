#include "Nimbus.h"

#include "Core/Event/Event.h"
#include "Core/Event/KeyEvent.h"
#include "Core/Event/MouseEvent.h"
#include "Core/Event/ResizeEvent.h"
#include "Platform/MacOS/MacOSInput.h"
#include "Platform/MacOS/MacOSWindow.h"

#include <print>

void Nimbus::CreateEngine()
{
    const auto input = MacOsInput{};
    EventBus::Instance().Get<ResizeEvent>().Register([](const ResizeEvent& e) {
        std::println("Width: {}, Height: {}", e.Width, e.Height);
    });

    EventBus::Instance().Get<KeyEvent>().Register([](const auto &e) {
        std::println("Key code: {}, Key state: {}", ToString(e.Code), ToString(e.State));
    });

    EventBus::Instance().Get<LeftClickDownEvent>().Register([&](const auto &e) {
        std::println("Position: {}", input.GetMousePosition());
    });

    auto macOsWindow = MacOsWindow{800, 600, "Nimbus Engine"};

    while (!macOsWindow.ShouldClose())
    {
        macOsWindow.PollEvents();
        EventBus::Instance().FlushAll();
    }
}

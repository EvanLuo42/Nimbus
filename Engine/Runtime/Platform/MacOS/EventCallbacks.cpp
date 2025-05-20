#include "Event/Event.h"
#include "Event/KeyEvent.h"
#include "Event/ResizeEvent.h"
#include "Include/MacOSPlatform.h"

void PublishKeyDownEvent(const uint16_t keycode)
{
    EventBus::Instance().Get<KeyEvent>().Dispatch({static_cast<KeyCode>(keycode), KeyState::Pressed});
}

void PublishKeyUpEvent(const uint16_t keycode)
{
    EventBus::Instance().Get<KeyEvent>().Dispatch({static_cast<KeyCode>(keycode), KeyState::Released});
}

void PublishResizeEvent(const double width, const double height)
{
    EventBus::Instance().Get<ResizeEvent>().QueueEvent({static_cast<int>(width), static_cast<int>(height)});
}
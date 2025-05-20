#include "Event/Event.h"
#include "Event/ResizeEvent.h"
#include "Include/MacOSPlatform.h"

void PublishPushKeyEvent(const uint16_t keycode)
{
    if (keycode == 49)
    {
        MacOSPlatform::Program::setTitle("Title");
    }
}

void PublishResizeEvent(const double width, const double height)
{
    EventBus::Instance().Get<ResizeEvent>().QueueEvent({static_cast<int>(width), static_cast<int>(height)});
}
#include "Core/Event/Event.h"
#include "Core/Event/KeyEvent.h"
#include "Core/Event/MouseEvent.h"
#include "Core/Event/ResizeEvent.h"
#include "Include/MacOSPlatform.h"

void PublishKeyDownEvent(const uint16_t keycode)
{
    EventBus::Instance().Get<KeyEvent>().QueueEvent({static_cast<KeyCode>(keycode), KeyState::Pressed});
}

void PublishKeyUpEvent(const uint16_t keycode)
{
    EventBus::Instance().Get<KeyEvent>().QueueEvent({static_cast<KeyCode>(keycode), KeyState::Released});
}

void PublishResizeEvent(const double width, const double height)
{
    EventBus::Instance().Get<ResizeEvent>().QueueEvent({static_cast<int>(width), static_cast<int>(height)});
}

void PublishLeftClickDown()
{
    EventBus::Instance().Get<LeftClickDownEvent>().QueueEvent({});
}

void PublishLeftClickUp()
{
    EventBus::Instance().Get<LeftClickUpEvent>().QueueEvent({});
}

void PublishRightClickDown()
{
    EventBus::Instance().Get<RightClickDownEvent>().QueueEvent({});
}

void PublishRightClickUp()
{
    EventBus::Instance().Get<RightClickUpEvent>().QueueEvent({});
}

void PublishMiddleClickDown()
{
    EventBus::Instance().Get<MiddleClickDownEvent>().QueueEvent({});
}

void PublishMiddleClickUp()
{
    EventBus::Instance().Get<MiddleClickUpEvent>().QueueEvent({});
}

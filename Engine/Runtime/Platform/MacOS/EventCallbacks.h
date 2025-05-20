#pragma once
#include <cstdint>

void PublishKeyDownEvent(uint16_t keycode);
void PublishKeyUpEvent(uint16_t keycode);
void PublishResizeEvent(double width, double height);

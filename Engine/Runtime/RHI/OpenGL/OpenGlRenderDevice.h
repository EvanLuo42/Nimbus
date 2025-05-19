#pragma once
#include "RHI/IRenderDevice.h"


class OpenGlRenderDevice : public IRenderDevice {
public:
    void InitWithWindow(const NativeWindowHandle &window) override;
};

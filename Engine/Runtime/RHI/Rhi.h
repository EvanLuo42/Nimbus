#pragma once
#include "IRenderDevice.h"
#include "Platform/Window/IWindow.h"

enum class RhiBackend {
    Vulkan,
    OpenGl
};

class Rhi {
public:
    static void Init(RhiBackend backend, NativeWindowHandle handle);
    static IRenderDevice* Get();

private:
    static inline std::unique_ptr<IRenderDevice> Device;
};

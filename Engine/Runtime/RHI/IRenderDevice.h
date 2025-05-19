#pragma once
#include <memory>

#include "Platform/Window/IWindow.h"

class IRenderDevice {
public:
    virtual ~IRenderDevice() = default;
    virtual void InitWithWindow(const NativeWindowHandle &window);

    virtual void Init() = 0;
    virtual void BeginFrame() = 0;
    virtual void EndFrame() = 0;

    virtual std::unique_ptr<class IGPUBuffer> CreateBuffer(...) = 0;
    virtual std::unique_ptr<class IShader> CreateShader(...) = 0;
    virtual std::unique_ptr<class ITexture> CreateTexture(...) = 0;
};

#include "Rhi.h"

#include <stdexcept>

#include "OpenGL/OpenGlRenderDevice.h"
#include "Vulkan/VulkanRenderDevice.h"

void Rhi::Init(const RhiBackend backend, const NativeWindowHandle handle) {
    switch (backend) {
        case RhiBackend::OpenGl:
            Device = std::make_unique<OpenGlRenderDevice>();
        case RhiBackend::Vulkan:
            Device = std::make_unique<VulkanRenderDevice>();
        default:
            throw std::runtime_error("Unsupported RHI backend");
    }
    Device->InitWithWindow(handle);
}

IRenderDevice *Rhi::Get() {
    return Device.get();
}

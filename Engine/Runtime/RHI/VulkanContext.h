#pragma once

#include "Platform/IWindow.h"

#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1

#ifdef __APPLE__
#define VK_USE_PLATFORM_METAL_EXT
#elif _WIN32
#define VK_USE_PLATFORM_WIN32_MVK
#endif

#include <vulkan/vulkan_raii.hpp>

class VulkanContext
{
    std::unique_ptr<vk::detail::DynamicLoader> Loader;
    std::optional<vk::raii::Context> Context;
    std::optional<vk::raii::Instance> Instance;
    std::optional<vk::raii::SurfaceKHR> Surface;
    std::optional<vk::raii::PhysicalDevice> PhysicalDevice;
    std::optional<vk::raii::Device> Device;
    std::optional<vk::raii::Queue> GraphicsQueue;

    uint32_t GraphicsQueueFamily = 0;

  public:
    VulkanContext(const NativeWindowHandle &nativeWindowHandle, const std::string &appName);

    [[nodiscard]] const vk::raii::Instance &GetInstance() const
    {
        return *Instance;
    }

    [[nodiscard]] vk::raii::Device &GetDevice()
    {
        return *Device;
    }

    [[nodiscard]] vk::raii::PhysicalDevice &GetPhysicalDevice()
    {
        return *PhysicalDevice;
    }

    [[nodiscard]] vk::raii::SurfaceKHR &GetSurface()
    {
        return *Surface;
    }

    [[nodiscard]] vk::raii::Queue &GetGraphicsQueue()
    {
        return *GraphicsQueue;
    }

    [[nodiscard]] uint32_t GetGraphicsQueueFamily() const
    {
        return GraphicsQueueFamily;
    }
};

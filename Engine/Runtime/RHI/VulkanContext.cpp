#include "VulkanContext.h"

#include <iostream>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

VulkanContext::VulkanContext(const NativeWindowHandle &nativeWindowHandle, const std::string &appName)
{
    Loader = std::make_unique<vk::detail::DynamicLoader>();
    VULKAN_HPP_DEFAULT_DISPATCHER.init(Loader->getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr"));
    Context.emplace();
    vk::ApplicationInfo appInfo{appName.c_str(), 1, "Nimbus Engine", 1, VK_API_VERSION_1_3};
    std::vector extensions = {VK_KHR_SURFACE_EXTENSION_NAME,
#ifdef __APPLE__
                              VK_EXT_METAL_SURFACE_EXTENSION_NAME,
                              VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME
#endif
    };
    vk::InstanceCreateInfo createInfo{vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR,
                                      &appInfo,
                                      0,
                                      nullptr,
                                      static_cast<uint32_t>(extensions.size()),
                                      extensions.data()};
    try
    {
        Instance.emplace(*Context, createInfo);
    }
    catch (const vk::SystemError &err)
    {
        std::cerr << "[Vulkan] Instance creation failed: " << err.what() << std::endl;
        exit(-1);
    }
    VULKAN_HPP_DEFAULT_DISPATCHER.init(**Instance);
    switch (nativeWindowHandle.Type)
    {
    case NativeWindowHandle::Type::Cocoa: {
        const vk::MetalSurfaceCreateInfoEXT sci{{}, nativeWindowHandle.Handle};
        Surface.emplace(*Instance, sci);
        break;
    }
    default:
        break;
    }

    std::vector requiredExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    for (auto &device : Instance->enumeratePhysicalDevices())
    {
        auto props = device.getQueueFamilyProperties();
        auto supportedExts = device.enumerateDeviceExtensionProperties();

        bool allSupported = true;
        for (const auto & required : requiredExtensions) {
            bool found = false;
            for (auto& ext : supportedExts) {
                if (strcmp(required, ext.extensionName) == 0) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                allSupported = false;
                break;
            }
        }

        if (!allSupported) continue;

        for (uint32_t i = 0; i < props.size(); ++i) {
            if (props[i].queueFlags & vk::QueueFlagBits::eGraphics && device.getSurfaceSupportKHR(i, *Surface)) {
                PhysicalDevice.emplace(std::move(device));
                GraphicsQueueFamily = i;
                goto Found;
            }
        }
    }
    throw std::runtime_error("No suitable GPU found");
Found:
    auto priority = 1.0f;
    vk::DeviceQueueCreateInfo queueInfo{{}, GraphicsQueueFamily, 1, &priority};
    vk::DeviceCreateInfo deviceInfo{{}, 1, &queueInfo, 0, nullptr, static_cast<uint32_t>(requiredExtensions.size()), requiredExtensions.data()};
    Device.emplace(*PhysicalDevice, deviceInfo);
    VULKAN_HPP_DEFAULT_DISPATCHER.init(**Device);
    GraphicsQueue.emplace(Device->getQueue(GraphicsQueueFamily, 0));
}
#include "VulkanContext.h"

#include "Core/Utils/LogUtils.h"

#include "Core/Utils/VectorUtils.h"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

VulkanContext::VulkanContext(const NativeWindowHandle &nativeWindowHandle, const std::string &appName)
{
    Logger = GetLogger("Vulkan");

    Loader = std::make_unique<vk::detail::DynamicLoader>();
    VULKAN_HPP_DEFAULT_DISPATCHER.init(Loader->getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr"));
    Context.emplace();

    vk::ApplicationInfo appInfo{appName.c_str(), 1, "Nimbus Engine", 1, VK_API_VERSION_1_3};
    std::vector extensions = {VK_KHR_SURFACE_EXTENSION_NAME,
#ifdef __APPLE__
                              VK_EXT_METAL_SURFACE_EXTENSION_NAME, VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME
#endif
    };
    NB_DEBUG("Creating app {} Vulkan instance using extensions: {}", appName, JoinVector(extensions));
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
        NB_ERROR("Instance creation failed: ", err.what());
        exit(-1);
    }
    VULKAN_HPP_DEFAULT_DISPATCHER.init(**Instance);
    switch (nativeWindowHandle.Type)
    {
    case NativeWindowHandle::Type::Cocoa: {
        const vk::MetalSurfaceCreateInfoEXT sci{{}, nativeWindowHandle.Handle};
        try
        {
            Surface.emplace(*Instance, sci);
        }
        catch (const vk::SystemError &err)
        {
            NB_ERROR("Surface creation failed: ", err.what());
            exit(-1);
        }
        break;
    }
    default:
        break;
    }

    std::vector requiredExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    for (auto &device : Instance->enumeratePhysicalDevices())
    {
        auto props = device.getQueueFamilyProperties();
        auto supportedExtensions = device.enumerateDeviceExtensionProperties();

        const bool allSupported = std::ranges::all_of(requiredExtensions, [&](const char *required) {
            return std::ranges::any_of(supportedExtensions,
                                       [&](const auto &ext) { return std::strcmp(required, ext.extensionName) == 0; });
        });

        if (!allSupported)
            continue;

        for (uint32_t i = 0; i < props.size(); ++i)
        {
            if (props[i].queueFlags & vk::QueueFlagBits::eGraphics && device.getSurfaceSupportKHR(i, *Surface))
            {
                PhysicalDevice.emplace(std::move(device));
                GraphicsQueueFamily = i;
                goto Found;
            }
        }
    }
    NB_ERROR("No suitable GPU found");
    exit(-1);
Found:
    auto priority = 1.0f;
    vk::DeviceQueueCreateInfo queueInfo{{}, GraphicsQueueFamily, 1, &priority};
    vk::DeviceCreateInfo deviceInfo{
        {}, 1, &queueInfo, 0, nullptr, static_cast<uint32_t>(requiredExtensions.size()), requiredExtensions.data()};
    try
    {
        Device.emplace(*PhysicalDevice, deviceInfo);
    }
    catch (const vk::SystemError &err)
    {
        NB_ERROR("Device creation failed: ", err.what());
        exit(-1);
    }
    VULKAN_HPP_DEFAULT_DISPATCHER.init(**Device);
    try
    {
        GraphicsQueue.emplace(Device->getQueue(GraphicsQueueFamily, 0));
    }
    catch (const vk::SystemError &err)
    {
        NB_ERROR("Graphics queue creation failed: ", err.what());
        exit(-1);
    }
}
#pragma once

#include <iostream>
#include <vulkan/vulkan_raii.hpp>

#include "GLFW/glfw3.h"

namespace Nimbus::Rendering
{
struct VulkanInstanceInfo {
    vk::raii::Context context;
    vk::raii::Instance instance{nullptr};
    vk::raii::DebugUtilsMessengerEXT debugMessenger{nullptr};
    std::vector<const char*> enabledExtensions;
    std::vector<const char*> enabledLayers;
};

class VulkanInstanceBuilder
{
public:
    explicit VulkanInstanceBuilder(const char* appName = "Nimbus Renderer",
                                   const uint32_t appVersion = VK_MAKE_VERSION(1, 0, 0))
    {
        appInfo = vk::ApplicationInfo{
            .pApplicationName = appName,
            .applicationVersion = appVersion,
            .pEngineName = "Nimbus",
            .engineVersion = VK_MAKE_VERSION(1, 0, 0),
            .apiVersion = vk::ApiVersion14,
        };

        uint32_t glfwExtCount = 0;
        const char** glfwExts = glfwGetRequiredInstanceExtensions(&glfwExtCount);
        extensions.assign(glfwExts, glfwExts + glfwExtCount);

#ifdef __APPLE__
        extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
        extensions.push_back(VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME);
        extensions.push_back("VK_MVK_macos_surface");
#endif
    }

    VulkanInstanceBuilder& enableValidation(const bool enable = true)
    {
        enableValidationLayers = enable;
#ifndef NDEBUG
        if (enable)
        {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            layers.push_back("VK_LAYER_KHRONOS_validation");
        }
#endif
        return *this;
    }

    VulkanInstanceBuilder& addExtension(const char* ext)
    {
        extensions.push_back(ext);
        return *this;
    }

    VulkanInstanceBuilder& addLayer(const char* layer)
    {
        layers.push_back(layer);
        return *this;
    }

    VulkanInstanceInfo build()
    {
        VulkanInstanceInfo info;
        const auto& context = info.context;

        auto supportedExts = context.enumerateInstanceExtensionProperties();
        for (auto ext : extensions)
        {
            if (std::ranges::none_of(supportedExts, [ext](auto const& e)
                                     { return strcmp(e.extensionName, ext) == 0; }))
            {
                throw std::runtime_error("Missing required Vulkan extension: " + std::string(ext));
            }
        }

        if (enableValidationLayers)
        {
            auto supportedLayers = context.enumerateInstanceLayerProperties();
            for (auto layer : layers)
            {
                if (std::ranges::none_of(supportedLayers, [layer](auto const& l)
                                         { return strcmp(l.layerName, layer) == 0; }))
                {
                    throw std::runtime_error("Missing validation layer: " + std::string(layer));
                }
            }
        }

        // Create Instance
        vk::InstanceCreateFlags flags{};
#ifdef __APPLE__
        flags |= vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR;
#endif

        const vk::InstanceCreateInfo createInfo{
            .flags = flags,
            .pApplicationInfo = &appInfo,
            .enabledLayerCount = static_cast<uint32_t>(layers.size()),
            .ppEnabledLayerNames = layers.data(),
            .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
            .ppEnabledExtensionNames = extensions.data(),
        };

        info.instance = vk::raii::Instance(context, createInfo);
        info.enabledExtensions = extensions;
        info.enabledLayers = layers;

        if (enableValidationLayers)
            setupDebugMessenger(info);

        return info;
    }

private:
    static void setupDebugMessenger(VulkanInstanceInfo& info)
    {
        constexpr vk::DebugUtilsMessengerCreateInfoEXT debugInfo{
            .messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                vk::DebugUtilsMessageSeverityFlagBitsEXT::eError |
                vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo,
            .messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
                vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
            .pfnUserCallback = &debugCallback,
        };

        info.debugMessenger = vk::raii::DebugUtilsMessengerEXT(info.instance, debugInfo);
    }

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(const vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
        vk::DebugUtilsMessageTypeFlagsEXT types,
        const vk::DebugUtilsMessengerCallbackDataEXT* callbackData,
        void* /*userData*/)
    {
        // TODO: Implement logging
        std::string prefix;
        std::ostream* out;

        switch (severity)
        {
        case vk::DebugUtilsMessageSeverityFlagBitsEXT::eError:
            prefix = "[Vulkan][Error] ";
            out = &std::cerr;
            break;
        case vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning:
            prefix = "[Vulkan][Warning] ";
            out = &std::cerr;
            break;
        case vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo:
            prefix = "[Vulkan][Info] ";
            out = &std::cout;
            break;
        case vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose:
            prefix = "[Vulkan][Verbose] ";
            out = &std::cout;
            break;
        default:
            prefix = "[Vulkan] ";
            out = &std::cout;
            break;
        }

        *out << prefix << callbackData->pMessage << std::endl;
        return VK_FALSE;
    }

    bool enableValidationLayers = false;
    vk::ApplicationInfo appInfo{};
    std::vector<const char*> extensions;
    std::vector<const char*> layers;
};
}

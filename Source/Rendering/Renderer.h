#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#include <__ostream/basic_ostream.h>
#include <iostream>
#include <vulkan/vulkan_raii.hpp>
#include "../Platform/Window.h"
#include "RenderGraph.h"

#include "../Utils/ArgParser.h"

namespace Nimbus::Rendering
{
class Renderer
{
public:
    explicit Renderer(const Platform::Window& window, const Utils::ArgParser& args) : window{window}, args{args}
    {
        const vk::ApplicationInfo appInfo{.pApplicationName = window.getTitle().data(),
                                          .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
                                          .pEngineName = "Nimbus",
                                          .engineVersion = VK_MAKE_VERSION(1, 0, 0),
                                          .apiVersion = vk::ApiVersion14};
        uint32_t glfwExtensionCount = 0;
        auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

#ifdef __APPLE__
        extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
        extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        extensions.push_back(VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME);
        extensions.push_back("VK_MVK_macos_surface");
#endif

#ifndef NDEBUG
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

        auto extensionProperties = context.enumerateInstanceExtensionProperties();
        for (const auto* ext : extensions)
        {
            if (std::ranges::none_of(extensionProperties,
                                     [ext](auto const& e) { return strcmp(e.extensionName, ext) == 0; }))
            {
                throw std::runtime_error("Required extension not supported: " + std::string(ext));
            }
        }

        // --- Create instance ---
        vk::InstanceCreateFlags instanceFlags{};

        const std::vector validationLayers = {"VK_LAYER_KHRONOS_validation"};

#ifdef __APPLE__
        instanceFlags |= vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR;
#endif

#ifdef NDEBUG
        constexpr bool enableValidationLayers = false;
#else
        constexpr bool enableValidationLayers = true;
#endif

        std::vector<char const*> requiredLayers;
        if (enableValidationLayers)
        {
            requiredLayers.assign(validationLayers.begin(), validationLayers.end());
        }

        if (enableValidationLayers)
        {
            auto layerProperties = context.enumerateInstanceLayerProperties();
            for (const auto* requiredLayer : requiredLayers)
            {
                if (std::ranges::none_of(layerProperties, [requiredLayer](auto const& layerProperty)
                                         { return strcmp(layerProperty.layerName, requiredLayer) == 0; }))
                {
                    throw std::runtime_error("Validation layer not supported: " + std::string(requiredLayer));
                }
            }
        }

        if (auto layerProperties = context.enumerateInstanceLayerProperties(); std::ranges::any_of(
                requiredLayers,
                [&layerProperties](auto const& requiredLayer)
                {
                    return std::ranges::none_of(layerProperties, [requiredLayer](auto const& layerProperty)
                                                { return strcmp(layerProperty.layerName, requiredLayer) == 0; });
                }))
        {
            throw std::runtime_error("One or more required layers are not supported!");
        }

        const vk::InstanceCreateInfo createInfo{.pApplicationInfo = &appInfo,
                                                .flags = instanceFlags,
                                                .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
                                                .ppEnabledExtensionNames = extensions.data(),
                                                .ppEnabledLayerNames = requiredLayers.data(),
                                                .enabledLayerCount = static_cast<uint32_t>(requiredLayers.size())};
        instance = std::make_unique<vk::raii::Instance>(context, createInfo);

#ifndef NDEBUG
        setupDebugMessenger();
#endif

        VkSurfaceKHR _surface;
        if (glfwCreateWindowSurface(**instance, window.getNativeHandle(), nullptr, &_surface) != 0)
        {
            throw std::runtime_error("failed to create window surface!");
        }
        surface = std::make_unique<vk::raii::SurfaceKHR>(*instance, _surface);
    }

    void render();
    void buildGraph();

private:
    void setupDebugMessenger()
    {
        constexpr vk::DebugUtilsMessengerCreateInfoEXT debugCreateInfo{
            .messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                vk::DebugUtilsMessageSeverityFlagBitsEXT::eError | vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo,
            .messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
            .pfnUserCallback = &debugCallback};

        debugMessenger = std::make_unique<vk::raii::DebugUtilsMessengerEXT>(*instance, debugCreateInfo);
    }

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                        vk::DebugUtilsMessageTypeFlagsEXT messageType,
                                                        const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                        void* /*pUserData*/)
    {
        std::cerr << "[Vulkan] " << pCallbackData->pMessage << std::endl;
        return VK_FALSE;
    }

    RenderGraph renderGraph;
    const Platform::Window& window;
    const Utils::ArgParser& args;

    vk::raii::Context context;
    std::unique_ptr<vk::raii::Instance> instance;
    std::unique_ptr<vk::raii::SurfaceKHR> surface;
    std::unique_ptr<vk::raii::DebugUtilsMessengerEXT> debugMessenger;
};
} // namespace Nimbus::Rendering

#pragma once
#include <iostream>
#include <optional>
#include <vulkan/vulkan_raii.hpp>

namespace Nimbus::Rendering
{
struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    [[nodiscard]] bool isComplete() const {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct PhysicalDeviceInfo {
    vk::raii::PhysicalDevice physicalDevice{nullptr};
    QueueFamilyIndices indices;
};

class PhysicalDeviceSelector {
public:
    PhysicalDeviceSelector(const vk::raii::Instance& instance, const vk::SurfaceKHR& surface)
        : instance(instance), surface(surface) {}

    [[nodiscard]] PhysicalDeviceInfo pickBestDevice() const
    {
        for (auto& device : instance.enumeratePhysicalDevices())
        {
            if (isDeviceSuitable(device))
            {
                // TODO: Implement logging
                std::cout << "[Nimbus] Selected GPU: "
                          << device.getProperties().deviceName << std::endl;
                return {vk::raii::PhysicalDevice{device}, findQueueFamilies(device)};
            }
        }
        throw std::runtime_error("Failed to find a suitable GPU!");
    }

private:
    [[nodiscard]] bool isDeviceSuitable(const vk::PhysicalDevice& device) const
    {
        const auto indices = findQueueFamilies(device);

        return indices.isComplete();
    }

    [[nodiscard]] QueueFamilyIndices findQueueFamilies(const vk::PhysicalDevice& device) const
    {
        QueueFamilyIndices indices;
        const auto queueFamilies = device.getQueueFamilyProperties();

        for (uint32_t i = 0; i < queueFamilies.size(); ++i)
        {
            if (queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics)
                indices.graphicsFamily = i;

            if (device.getSurfaceSupportKHR(i, surface))
                indices.presentFamily = i;

            if (indices.isComplete())
                break;
        }

        return indices;
    }

    const vk::raii::Instance& instance;
    const vk::SurfaceKHR& surface;
};
}

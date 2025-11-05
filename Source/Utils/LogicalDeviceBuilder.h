#pragma once

#include <set>

#include "PhysicalDeviceSelector.h"

namespace Nimbus::Rendering
{
struct LogicalDeviceInfo
{
    vk::raii::Device device{nullptr};
    vk::raii::Queue graphicsQueue{nullptr};
    vk::raii::Queue presentQueue{nullptr};
};

class LogicalDeviceBuilder
{
public:
    LogicalDeviceBuilder(const vk::raii::PhysicalDevice& physicalDevice, const QueueFamilyIndices& indices) :
        physicalDevice(physicalDevice), indices(indices)
    {
    }

    LogicalDeviceBuilder& addExtension(const char* ext)
    {
        extensions.push_back(ext);
        return *this;
    }

    LogicalDeviceInfo build()
    {
        std::set uniqueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
        float queuePriority = 1.0f;

        for (uint32_t queueFamily : uniqueFamilies)
        {
            vk::DeviceQueueCreateInfo queueCreateInfo{
                .queueFamilyIndex = queueFamily, .queueCount = 1, .pQueuePriorities = &queuePriority};
            queueCreateInfos.push_back(queueCreateInfo);
        }

        vk::PhysicalDeviceFeatures deviceFeatures{};

#ifdef __APPLE__
        extensions.push_back("VK_KHR_portability_subset");
#endif

        vk::PhysicalDeviceDynamicRenderingFeatures dynamicRenderingFeature{.dynamicRendering = VK_TRUE};
        vk::PhysicalDeviceSynchronization2Features sync2Feature{.pNext = &dynamicRenderingFeature,
                                                                .synchronization2 = VK_TRUE};
        vk::DeviceCreateInfo createInfo{
            .pNext = &sync2Feature,
            .queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size()),
            .pQueueCreateInfos = queueCreateInfos.data(),
            .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
            .ppEnabledExtensionNames = extensions.data(),
            .pEnabledFeatures = &deviceFeatures,
        };

        vk::raii::Device device{physicalDevice, createInfo};

        vk::raii::Queue graphicsQueue{device, indices.graphicsFamily.value(), 0};
        vk::raii::Queue presentQueue{device, indices.presentFamily.value(), 0};

        // TODO: Implement logging
        std::cout << "[Nimbus] Logical device and queues created successfully." << std::endl;

        return {std::move(device), std::move(graphicsQueue), std::move(presentQueue)};
    }

private:
    const vk::raii::PhysicalDevice& physicalDevice;
    const QueueFamilyIndices& indices;
    std::vector<const char*> extensions;
    bool enableValidationLayers = false;
};
} // namespace Nimbus::Rendering

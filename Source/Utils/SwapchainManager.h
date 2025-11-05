#pragma once
#include <algorithm>
#include <vulkan/vulkan_raii.hpp>

#include "GLFW/glfw3.h"
#include "PhysicalDeviceSelector.h"

namespace vk
{
class PhysicalDevice;
}
namespace vk::raii
{
class Device;
class SurfaceKHR;
}
namespace Nimbus::Rendering
{
struct QueueFamilyIndices;
struct SwapchainInfo {
    vk::raii::SwapchainKHR swapchain{nullptr};
    std::vector<vk::raii::ImageView> imageViews;
    vk::Format imageFormat{};
    vk::Extent2D extent{};
};

class SwapchainManager
{
public:
    SwapchainManager(
        const vk::raii::Device& device,
        const vk::PhysicalDevice& physicalDevice,
        const vk::raii::SurfaceKHR& surface,
        const QueueFamilyIndices& indices,
        GLFWwindow* window)
        : device(device), physicalDevice(physicalDevice), surface(surface), indices(indices), window(window)
    {}

    SwapchainInfo create()
    {
        auto [capabilities, formats, presentModes] = querySwapchainSupport();

        vk::SurfaceFormatKHR surfaceFormat = chooseSurfaceFormat(formats);
        vk::PresentModeKHR presentMode = choosePresentMode(presentModes);
        vk::Extent2D extent = chooseExtent(capabilities);

        uint32_t imageCount = capabilities.minImageCount + 1;
        if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount)
            imageCount = capabilities.maxImageCount;

        vk::SwapchainCreateInfoKHR createInfo{
            .surface = *surface,
            .minImageCount = imageCount,
            .imageFormat = surfaceFormat.format,
            .imageColorSpace = surfaceFormat.colorSpace,
            .imageExtent = extent,
            .imageArrayLayers = 1,
            .imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
        };

        uint32_t queueFamilyIndices[] = {
            indices.graphicsFamily.value(),
            indices.presentFamily.value()
        };

        if (indices.graphicsFamily != indices.presentFamily)
        {
            createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else
        {
            createInfo.imageSharingMode = vk::SharingMode::eExclusive;
        }

        createInfo.preTransform = capabilities.currentTransform;
        createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;

        vk::raii::SwapchainKHR swapchain{device, createInfo};

        std::vector<vk::Image> images = swapchain.getImages();

        std::vector<vk::raii::ImageView> imageViews;
        imageViews.reserve(images.size());

        for (auto& image : images)
        {
            vk::ImageViewCreateInfo viewInfo{
                .image = image,
                .viewType = vk::ImageViewType::e2D,
                .format = surfaceFormat.format,
                .components = {
                    vk::ComponentSwizzle::eIdentity,
                    vk::ComponentSwizzle::eIdentity,
                    vk::ComponentSwizzle::eIdentity,
                    vk::ComponentSwizzle::eIdentity,
                },
                .subresourceRange = {
                    vk::ImageAspectFlagBits::eColor,
                    0, 1, 0, 1
                }
            };
            imageViews.emplace_back(device, viewInfo);
        }

        // TODO: Implement logging
        std::cout << "[Nimbus] Swapchain created with " << images.size() << " images ("
                  << extent.width << "x" << extent.height << ")" << std::endl;

        return {
            std::move(swapchain),
            std::move(imageViews),
            surfaceFormat.format,
            extent
        };
    }

private:
    struct SwapchainSupportDetails {
        vk::SurfaceCapabilitiesKHR capabilities;
        std::vector<vk::SurfaceFormatKHR> formats;
        std::vector<vk::PresentModeKHR> presentModes;
    };

    SwapchainSupportDetails querySwapchainSupport() const
    {
        SwapchainSupportDetails details;
        details.capabilities = physicalDevice.getSurfaceCapabilitiesKHR(*surface);
        details.formats = physicalDevice.getSurfaceFormatsKHR(*surface);
        details.presentModes = physicalDevice.getSurfacePresentModesKHR(*surface);
        return details;
    }

    static vk::SurfaceFormatKHR chooseSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& formats)
    {
        for (const auto& available : formats)
        {
            if (available.format == vk::Format::eB8G8R8A8Srgb &&
                available.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
                return available;
        }
        return formats[0];
    }

    static vk::PresentModeKHR choosePresentMode(const std::vector<vk::PresentModeKHR>& modes)
    {
        for (const auto& mode : modes)
        {
            if (mode == vk::PresentModeKHR::eMailbox)
                return mode;
        }
        return vk::PresentModeKHR::eFifo;
    }

    vk::Extent2D chooseExtent(const vk::SurfaceCapabilitiesKHR& capabilities) const
    {
        if (capabilities.currentExtent.width != UINT32_MAX)
            return capabilities.currentExtent;

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        vk::Extent2D actualExtent{
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::clamp(
            actualExtent.width,
            capabilities.minImageExtent.width,
            capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(
            actualExtent.height,
            capabilities.minImageExtent.height,
            capabilities.maxImageExtent.height);
        return actualExtent;
    }

    const vk::raii::Device& device;
    const vk::PhysicalDevice& physicalDevice;
    const vk::raii::SurfaceKHR& surface;
    const QueueFamilyIndices& indices;
    GLFWwindow* window;
};
}
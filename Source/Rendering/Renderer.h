#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#include <vulkan/vulkan_raii.hpp>

#include "../Platform/Window.h"
#include "../RenderGraph/RenderGraph.h"
#include "../Utils/LogicalDeviceBuilder.h"
#include "../Utils/SwapchainManager.h"

#include "../Utils/ArgParser.h"
#include "../Utils/VulkanInstanceBuilder.h"

#include <vk_mem_alloc.h>

#ifndef NDEBUG
inline bool enableValidationLayer = true;
#else
inline bool enableValidationLayer = false;
#endif

namespace Nimbus::Rendering
{
class Renderer
{
public:
    explicit Renderer(const Platform::Window& window, const Utils::ArgParser& args) : window{window}, args{args}
    {
        createInstance();
        createSurface();
        createPhysicalDevice();
        createLogicalDevice();
        createSwapchain();
        createAllocator();
        createCommandPool();

        buildGraph();
    }

    ~Renderer()
    {
        if (allocator)
            vmaDestroyAllocator(allocator);
    }

    void render();
    void buildGraph();

private:
    void beginFrame();
    void endFrame();

    void createInstance()
    {
        VulkanInstanceBuilder builder{window.getTitle().data()};
        builder.enableValidation(enableValidationLayer);
        auto info = builder.build();
        instance = std::move(info.instance);
        if (*info.debugMessenger)
            debugMessenger = std::move(info.debugMessenger);
    }

    void createSurface()
    {
        VkSurfaceKHR _surface;
        if (glfwCreateWindowSurface(*instance, window.getNativeHandle(), nullptr, &_surface) != 0)
        {
            throw std::runtime_error("failed to create window surface!");
        }
        surface = vk::raii::SurfaceKHR{instance, _surface};
    }

    void createPhysicalDevice()
    {
        const PhysicalDeviceSelector selector{instance, surface};
        auto [physicalDevice_, indices] = selector.pickBestDevice();
        physicalDevice = vk::raii::PhysicalDevice{std::move(physicalDevice_)};
        queueIndices = indices;
    }

    void createLogicalDevice()
    {
        LogicalDeviceBuilder deviceBuilder{(physicalDevice), queueIndices};
        deviceBuilder.addExtension(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
        deviceBuilder.addExtension(VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME);
        auto [device_, graphicsQueue_, presentQueue_] = deviceBuilder.build();

        device = vk::raii::Device{std::move(device_)};
        graphicsQueue = vk::raii::Queue{std::move(graphicsQueue_)};
        presentQueue = vk::raii::Queue{std::move(presentQueue_)};
    }

    void createSwapchain()
    {
        SwapchainManager swapchainManager{device, physicalDevice, surface, queueIndices, window.getNativeHandle()};
        auto [swapchain_, imageViews, imageFormat, extent] = swapchainManager.create();

        swapchain = vk::raii::SwapchainKHR{std::move(swapchain_)};
        swapchainImageViews = std::move(imageViews);
        swapchainExtent = extent;
        swapchainFormat = imageFormat;
    }

    void createAllocator()
    {
        VmaAllocatorCreateInfo allocatorInfo = {};
        allocatorInfo.physicalDevice = *physicalDevice;
        allocatorInfo.device = *device;
        allocatorInfo.instance = *instance;
        allocator = {};
        vmaCreateAllocator(&allocatorInfo, &allocator);
        renderGraph = std::make_unique<RenderGraph::RenderGraph>(allocator);
    }

    void createCommandPool()
    {
        const vk::CommandPoolCreateInfo poolInfo{.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
                                                 .queueFamilyIndex = queueIndices.graphicsFamily.value()};
        commandPool = vk::raii::CommandPool{device, poolInfo};
    }

    const Platform::Window& window;
    const Utils::ArgParser& args;

    vk::raii::Context context;
    vk::raii::Instance instance{nullptr};
    vk::raii::DebugUtilsMessengerEXT debugMessenger{nullptr};
    vk::raii::PhysicalDevice physicalDevice{nullptr};
    vk::raii::Device device{nullptr};
    vk::raii::SurfaceKHR surface{nullptr};
    vk::raii::Queue graphicsQueue{nullptr};
    vk::raii::Queue presentQueue{nullptr};
    vk::raii::SwapchainKHR swapchain{nullptr};
    vk::raii::CommandPool commandPool{nullptr};
    VmaAllocator allocator{VK_NULL_HANDLE};
    std::vector<vk::raii::ImageView> swapchainImageViews;
    vk::Extent2D swapchainExtent{};
    vk::Format swapchainFormat{};
    QueueFamilyIndices queueIndices;
    std::unique_ptr<RenderGraph::RenderGraph> renderGraph;
};
} // namespace Nimbus::Rendering

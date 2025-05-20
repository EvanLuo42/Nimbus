#include "Nimbus.h"

#include "Core/Event/Event.h"
#include "Platform/MacOS/MacOSInput.h"
#include "Platform/MacOS/MacOSWindow.h"
#include "Platform/PlatformContext.h"

#include "RHI/VulkanContext.h"

#include <iostream>
#include <vulkan/vulkan_raii.hpp>

void Nimbus::CreateEngine()
{
    spdlog::set_level(spdlog::level::debug);

    std::unique_ptr<IWindow> window = std::make_unique<MacOsWindow>(800, 600, "Nimbus Engine");
    std::unique_ptr<IInput> input = std::make_unique<MacOsInput>();

    PlatformContext platformContext{std::move(window), std::move(input)};

    VulkanContext vk{platformContext.GetWindow()->GetNativeHandle(), "Nimbus Engine"};

    vk::raii::SurfaceKHR& surface = vk.GetSurface();
    vk::raii::Device& device = vk.GetDevice();
    vk::raii::PhysicalDevice& physical = vk.GetPhysicalDevice();

    auto surfaceCaps = physical.getSurfaceCapabilitiesKHR(*surface);
    auto formats = physical.getSurfaceFormatsKHR(*surface);
    auto chosenFormat = formats[0].format;

    vk::Extent2D extent = surfaceCaps.currentExtent;

    vk::SwapchainCreateInfoKHR swapchainInfo{};
    swapchainInfo.surface = *surface;
    swapchainInfo.minImageCount = surfaceCaps.minImageCount;
    swapchainInfo.imageFormat = chosenFormat;
    swapchainInfo.imageColorSpace = formats[0].colorSpace;
    swapchainInfo.imageExtent = extent;
    swapchainInfo.imageArrayLayers = 1;
    swapchainInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
    swapchainInfo.imageSharingMode = vk::SharingMode::eExclusive;
    swapchainInfo.preTransform = surfaceCaps.currentTransform;
    swapchainInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
    swapchainInfo.presentMode = vk::PresentModeKHR::eFifo;
    swapchainInfo.clipped = true;

    vk::raii::SwapchainKHR swapchain(device, swapchainInfo);
    auto images = swapchain.getImages();

    std::vector<vk::raii::ImageView> imageViews;
    for (auto& img : images) {
        vk::ImageViewCreateInfo viewInfo({}, img, vk::ImageViewType::e2D, chosenFormat,
                                         {}, {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1});
        imageViews.emplace_back(device, viewInfo);
    }

    vk::AttachmentDescription colorAttachment({}, chosenFormat,
        vk::SampleCountFlagBits::e1, vk::AttachmentLoadOp::eClear,
        vk::AttachmentStoreOp::eStore, vk::AttachmentLoadOp::eDontCare,
        vk::AttachmentStoreOp::eDontCare, vk::ImageLayout::eUndefined,
        vk::ImageLayout::ePresentSrcKHR);

    vk::AttachmentReference colorRef(0, vk::ImageLayout::eColorAttachmentOptimal);
    vk::SubpassDescription subpass({}, vk::PipelineBindPoint::eGraphics, 0, nullptr, 1, &colorRef);

    vk::RenderPassCreateInfo rpInfo({}, 1, &colorAttachment, 1, &subpass);
    vk::raii::RenderPass renderPass(device, rpInfo);

    std::vector<vk::raii::Framebuffer> framebuffers;
    for (auto& view : imageViews) {
        vk::FramebufferCreateInfo fbInfo({}, *renderPass, 1, &*view, extent.width, extent.height, 1);
        framebuffers.emplace_back(device, fbInfo);
    }

    uint32_t qf = vk.GetGraphicsQueueFamily();
    vk::CommandPoolCreateInfo poolInfo({}, qf);
    vk::raii::CommandPool commandPool(device, poolInfo);

    vk::CommandBufferAllocateInfo allocInfo(*commandPool, vk::CommandBufferLevel::ePrimary, 1);
    auto commandBuffers = device.allocateCommandBuffers(allocInfo);
    auto& cmd = commandBuffers[0];

    cmd.begin({vk::CommandBufferUsageFlagBits::eOneTimeSubmit});
    vk::ClearValue clearColor = vk::ClearColorValue(std::array{0.1f, 0.2f, 0.3f, 1.0f});
    vk::RenderPassBeginInfo rpbInfo(*renderPass, *framebuffers[0], {{0, 0}, extent}, 1, &clearColor);
    cmd.beginRenderPass(rpbInfo, vk::SubpassContents::eInline);
    cmd.endRenderPass();
    cmd.end();

    vk::raii::Queue queue = vk.GetGraphicsQueue();
    vk::CommandBuffer rawCmd = *cmd;
    vk::SubmitInfo submit({}, {}, rawCmd);
    queue.submit(submit);
    queue.waitIdle();

    uint32_t imageIndex = 0;
    std::array swapchains = {*swapchain};
    std::array imageIndices = {imageIndex};

    vk::PresentInfoKHR presentInfo(
        {},
        swapchains,
        imageIndices
    );
    queue.presentKHR(presentInfo);

    while (!platformContext.GetWindow()->ShouldClose())
    {
        platformContext.GetWindow()->PollEvents();
    }
}

#pragma once

#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#include <vulkan/vulkan_raii.hpp>

namespace Nimbus::RenderGraph
{
struct ResourceDesc {
    vk::Format format{};
    vk::Extent2D extent{};
    vk::ImageUsageFlags usage{};
    std::string name;

    bool compatibleWith(const ResourceDesc& other) const {
        return format == other.format &&
               extent.width == other.extent.width &&
               extent.height == other.extent.height &&
               usage == other.usage;
    }
};

using ResourceHandle = uint32_t;
}

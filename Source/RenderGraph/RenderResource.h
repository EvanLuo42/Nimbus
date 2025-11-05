#pragma once

#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#include <vulkan/vulkan_raii.hpp>

#include <glm/vec4.hpp>

namespace Nimbus::RenderGraph
{
struct ResourceDesc {
    vk::Format format{};
    vk::Extent2D extent{};
    vk::ImageUsageFlags usage{};
    std::string name;

    glm::vec4 clearColor{ 0, 0, 0, 1 };

    bool compatibleWith(const ResourceDesc& other) const {
        return format == other.format &&
               extent.width == other.extent.width &&
               extent.height == other.extent.height &&
               usage == other.usage;
    }
};

using ResourceHandle = uint32_t;
}

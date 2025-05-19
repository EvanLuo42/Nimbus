cmake_minimum_required(VERSION 3.31)

add_library(NimbusRuntime SHARED Nimbus.cpp
        Platform/Window/GLFWWindow.h
        Platform/Window/GLFWWindow.cpp
        Platform/Window/IWindow.h
        Platform/Input/GlfwInput.cpp
        Platform/Input/GlfwInput.h
        Platform/Input/Input.h
        Platform/Platform.cpp
        Platform/Platform.h
        RHI/IGpuBuffer.h
        RHI/IRenderDevice.h
        RHI/Rhi.cpp
        RHI/Rhi.h
        RHI/Vulkan/VulkanBuffer.h
        RHI/Vulkan/VulkanBuffer.cpp
        RHI/OpenGL/OpenGlRenderDevice.cpp
        RHI/OpenGL/OpenGlRenderDevice.h
        RHI/OpenGL/OpenGlBuffer.cpp
        RHI/OpenGL/OpenGlBuffer.h
)

target_include_directories(NimbusRuntime PUBLIC .)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(NimbusRuntime PRIVATE glfw)

find_package(glm CONFIG REQUIRED)
target_link_libraries(NimbusRuntime PRIVATE glm::glm)

find_package(glad CONFIG REQUIRED)
target_link_libraries(NimbusRuntime PRIVATE glad)

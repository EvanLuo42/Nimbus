cmake_minimum_required(VERSION 3.31)

add_library(MacOSPlatform STATIC
        Platform/MacOS/Swift/Platform.swift
)

set_target_properties(MacOSPlatform PROPERTIES Swift_MODULE_NAME "MacOSPlatform")
target_compile_options(MacOSPlatform PUBLIC
        "$<$<COMPILE_LANGUAGE:Swift>:-cxx-interoperability-mode=default>"
        "$<$<COMPILE_LANGUAGE:Swift>:-Xcc;-fmodule-map-file=${CMAKE_CURRENT_SOURCE_DIR}/Include/module.modulemap>"
        "$<$<COMPILE_LANGUAGE:Swift>:-Xcc;-I${CMAKE_CURRENT_SOURCE_DIR}/Platform/MacOS>"
)

get_target_property(SWIFT_SOURCES MacOSPlatform SOURCES)
list(TRANSFORM SWIFT_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")

set(SWIFT_MODULE_NAME MacOSPlatform)
set(CLHG_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Include/MacOSPlatform.h)

set(SDK_FLAGS "-sdk" "${CMAKE_OSX_SYSROOT}")

add_custom_command(
        OUTPUT ${CLHG_OUTPUT_PATH}
        COMMAND ${CMAKE_Swift_COMPILER} -typecheck
        -emit-clang-header-path ${CLHG_OUTPUT_PATH}
        -emit-module
        -module-name ${SWIFT_MODULE_NAME}
        -cxx-interoperability-mode=default
        -Xcc -fmodule-map-file=${CMAKE_CURRENT_SOURCE_DIR}/Include/module.modulemap
        -Xcc -I${CMAKE_CURRENT_SOURCE_DIR}/Platform/MacOS
        ${SDK_FLAGS}
        ${SWIFT_SOURCES}
        DEPENDS ${SWIFT_SOURCES}
        COMMENT "Generating Swift Clang header for ${SWIFT_MODULE_NAME}"
        VERBATIM
)

target_include_directories(MacOSPlatform PUBLIC)

add_library(NimbusRuntime SHARED Nimbus.cpp
        Platform/IWindow.h
        Platform/IInput.h
        Platform/PlatformContext.h
        Include/MacOSPlatform.h
        Platform/MacOS/MacOSWindow.cpp
        Platform/MacOS/MacOSWindow.h
        Platform/MacOS/EventCallbacks.h
        Platform/MacOS/EventCallbacks.cpp
        Core/Event/Event.h
        Core/Event/ResizeEvent.h
        Core/Event/KeyEvent.h
        Platform/MacOS/MacOSInput.cpp
        Platform/MacOS/MacOSInput.h
        Core/Event/MouseEvent.h
        Renderer/Renderer.cpp
        Renderer/Renderer.h
        Scene/Scene.cpp
        Scene/Scene.h
        Scripting/Scripting.cpp
        Scripting/Scripting.h
        UI/UI.cpp
        UI/UI.h
        Resources/Resources.cpp
        Resources/Resources.h
        Physics/Physics.cpp
        Physics/Physics.h
        Audio/Audio.cpp
        Audio/Audio.h
        Platform/PlatformContext.cpp
        RHI/VulkanContext.cpp
        RHI/VulkanContext.h
        Core/Utils/LogUtils.h
        Core/Utils/VectorUtils.h
        Core/Utils/LogUtils.cpp
)

target_link_libraries(NimbusRuntime PRIVATE MacOSPlatform)

target_include_directories(NimbusRuntime PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Vulkan REQUIRED)
target_link_libraries(NimbusRuntime PRIVATE Vulkan::Vulkan)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(NimbusRuntime PRIVATE spdlog::spdlog)

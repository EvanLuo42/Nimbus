cmake_minimum_required(VERSION 3.31)

add_library(MacOSPlatform STATIC
        Platform/MacOS/Swift/Platform.swift
)

get_target_property(SWIFT_SOURCES MacOSPlatform SOURCES)
list(TRANSFORM SWIFT_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")

set(SWIFT_MODULE_NAME MacOSPlatform)
set(CLHG_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Include/MacOSPlatform.h)

set(SDK_FLAGS "-sdk" "${CMAKE_OSX_SYSROOT}")

add_custom_command(
        OUTPUT ${CLHG_OUTPUT_PATH}
        COMMAND ${CMAKE_Swift_COMPILER}
        -emit-clang-header-path ${CLHG_OUTPUT_PATH}
        -emit-module
        -module-name ${SWIFT_MODULE_NAME}
        -cxx-interoperability-mode=default
        ${SDK_FLAGS}
        ${SWIFT_SOURCES}
        DEPENDS ${SWIFT_SOURCES}
        COMMENT "Generating Swift Clang header for ${SWIFT_MODULE_NAME}"
        VERBATIM
)

add_library(NimbusRuntime SHARED Nimbus.cpp
        Platform/IWindow.h
        Platform/IInput.h
        Platform/PlatformContext.h
        Include/MacOSPlatform.h
)

target_link_libraries(NimbusRuntime PRIVATE MacOSPlatform)

target_include_directories(NimbusRuntime PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

find_package(glm CONFIG REQUIRED)
target_link_libraries(NimbusRuntime PRIVATE glm::glm)

find_package(Vulkan REQUIRED)
target_link_libraries(NimbusRuntime PRIVATE Vulkan::Vulkan)

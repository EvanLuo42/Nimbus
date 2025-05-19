cmake_minimum_required(VERSION 3.31)

add_library(NimbusRuntime SHARED Nimbus.cpp)

target_include_directories(NimbusRuntime PUBLIC .)
target_link_libraries(NimbusRuntime PRIVATE dawn)

find_package(glm CONFIG REQUIRED)
target_link_libraries(NimbusRuntime PRIVATE glm::glm)

find_package(Vulkan REQUIRED)
target_link_libraries(NimbusRuntime PRIVATE Vulkan::Vulkan)

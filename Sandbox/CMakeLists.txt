cmake_minimum_required(VERSION 3.31)

add_executable(Sandbox main.cpp
)

target_link_libraries(Sandbox PRIVATE NimbusRuntime)

target_include_directories(Sandbox PRIVATE ${CMAKE_SOURCE_DIR}/Engine/Runtime)

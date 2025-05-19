cmake_minimum_required(VERSION 3.31)

add_executable(NimbusEditor main.cpp)

target_link_libraries(NimbusEditor PRIVATE NimbusRuntime)

target_include_directories(NimbusEditor PRIVATE ${CMAKE_SOURCE_DIR}/Runtime)

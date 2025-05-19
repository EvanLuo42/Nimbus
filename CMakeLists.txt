cmake_minimum_required(VERSION 3.31)
project(NimbusEngine LANGUAGES CXX Swift)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(Engine/Runtime)
add_subdirectory(Engine/Editor)
add_subdirectory(Sandbox)
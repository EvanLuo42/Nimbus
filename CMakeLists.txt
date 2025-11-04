# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("Nimbus")

add_subdirectory("External")

file(GLOB_RECURSE SOURCES
	CONFIGURE_DEPENDS
	"Source/*.cpp"
    "Source/*.h"
)

include(${CMAKE_SOURCE_DIR}/cmake/CompileSlangShader.cmake)

set(SHADER_OUTPUT_DIR "${CMAKE_BINARY_DIR}/Shaders")
file(GLOB SHADER_SLANG_FILES "${CMAKE_SOURCE_DIR}/Shaders/*.slang")
file(GLOB SHADER_H_FILES "${CMAKE_SOURCE_DIR}/Shaders/*.h")

compile_slang(
  ${SHADER_SLANG_FILES}
  ${SHADER_OUTPUT_DIR}
  GENERATED_SHADER_HEADERS
)

add_executable(Nimbus
	${SOURCES}
		Source/Rendering/Renderer.cpp
		Source/Rendering/Renderer.h
		Source/Rendering/RenderGraph.cpp
		Source/Rendering/RenderGraph.h
)

add_dependencies(Nimbus compile_slang_shaders)

find_package(Vulkan REQUIRED)

target_link_libraries(Nimbus 
	PRIVATE 
	glfw
	Vulkan::Vulkan
	Slang
)

if (WIN32)
	set_target_properties(Nimbus PROPERTIES
		LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
	)
endif()
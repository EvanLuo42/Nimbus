project(External LANGUAGES C CXX)

if(NOT TARGET glfw)
  set(GLFW_USE_HYBRID_HPG ON)
  set(GLFW_VULKAN_STATIC OFF)
  set(GLFW_BUILD_DOCS OFF)
  set(GLFW_BUILD_EXAMPLES OFF)
  set(GLFW_BUILD_TESTS OFF)
  set(GLFW_BUILD_WAYLAND OFF)
  set(GLFW_INSTALL OFF)
  add_subdirectory(glfw)
  set_target_properties(glfw update_mappings PROPERTIES FOLDER "External/GLFW")
endif()

#if (NOT TARGET Slang)
#	add_library(Slang INTERFACE)
#	target_include_directories(Slang INTERFACE
#        "${Vulkan_INCLUDE_DIR}/slang"
#    )
#
#    if (WIN32)
#        target_link_libraries(Slang INTERFACE "${Vulkan_LIBRARY}/../slang.lib")
#    elseif(APPLE)
#        target_link_libraries(Slang INTERFACE "${Vulkan_LIBRARY}/../libslang.dylib")
#    else()
#        target_link_libraries(Slang INTERFACE "${Vulkan_LIBRARY}/../libslang.so")
#    endif()
#
#    set_target_properties(Slang PROPERTIES
#        SLANGC_EXECUTABLE "${SLANGC_EXECUTABLE}"
#    )
#endif()

if(NOT TARGET vma)
  add_subdirectory(vma)
endif()

add_subdirectory(glm)
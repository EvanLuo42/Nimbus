# CMakeList.txt : CMake project for Core, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (Core "Core.cpp" "Core.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Core PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.

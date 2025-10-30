file(GLOB_RECURSE SOURCES
	CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

add_executable(App ${SOURCES})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_target_properties(App PROPERTIES WIN32_EXECUTABLE FALSE)
else()
    set_target_properties(App PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

target_link_libraries(App
	PRIVATE
		Nimbus::Core
		Nimbus::RenderGraph
)
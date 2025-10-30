file(GLOB APP_SOURCES "*.cpp" "*.h")

add_executable(Editor ${APP_SOURCES})

set_target_properties(Editor PROPERTIES
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(Editor
	PRIVATE
		DirectXRenderer::Core
		DirectXRenderer::RenderGraph
)
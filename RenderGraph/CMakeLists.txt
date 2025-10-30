file(GLOB_RECURSE RG_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/*.h"
)

file(GLOB_RECURSE RG_PUBLIC_HEADERS
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Inc/DXRRenderGraph/*.h"
)

add_library(RenderGraph STATIC)
target_sources(RenderGraph PRIVATE ${RG_SOURCES} ${RG_PUBLIC_HEADERS})

target_include_directories(RenderGraph
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Inc
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Src
)

add_library(DirectXRenderer::RenderGraph ALIAS RenderGraph)
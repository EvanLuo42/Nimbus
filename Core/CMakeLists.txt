file(GLOB_RECURSE CORE_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/*.h"
)

file(GLOB_RECURSE CORE_PUBLIC_HEADERS
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Inc/DXRCore/*.h"
)

add_library(Core STATIC)
target_sources(Core PRIVATE ${CORE_SOURCES} ${CORE_PUBLIC_HEADERS})

target_include_directories(Core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Inc
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Src
)

add_library(DirectXRenderer::Core ALIAS Core)
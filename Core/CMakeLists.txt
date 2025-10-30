file(GLOB_RECURSE SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/*.h"
)

file(GLOB_RECURSE PUBLIC_HEADER
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Inc/NimbusCore/*.h"
)

add_library(Core STATIC)
target_sources(Core PRIVATE ${SOURCES} ${PUBLIC_HEADER})

target_include_directories(Core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Inc
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Src
)

target_link_libraries(Core
    PUBLIC
        dxheaders
        d3d12
        dxgi
)

add_library(Nimbus::Core ALIAS Core)
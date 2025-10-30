file(GLOB_RECURSE SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/*.h"
)

file(GLOB_RECURSE PUBLIC_HEADER
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Inc/NimbusRHI/*.h"
)

add_library(RHI STATIC)
target_sources(RHI PRIVATE ${SOURCES} ${PUBLIC_HEADER})

target_include_directories(RHI
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Inc
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Src
)

target_link_libraries(RHI
    PUBLIC
        dxheaders
        d3d12
        dxgi
)

add_library(Nimbus::RHI ALIAS RHI)
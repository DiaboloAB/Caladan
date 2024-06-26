cmake_minimum_required(VERSION 3.10)

project(Caladan)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(glfw3 3.3 REQUIRED)

find_package(Vulkan REQUIRED)

set(SOURCES
    Main/Main.cpp
    Renderer/Renderer.cpp
    Renderer/Window/Window.cpp
)

# set(HEADERS
#     Renderer/Renderer.hpp
#     Renderer/Window/Window.hpp
# )

include_directories(
    ${CMAKE_SOURCE_DIR}/Renderer
    ${CMAKE_SOURCE_DIR}/Renderer/Window
)

add_executable(Caladan ${SOURCES} ${HEADERS})

target_link_libraries(Caladan glfw Vulkan::Vulkan)

add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/*
    COMMENT "Cleaning all build files"
)

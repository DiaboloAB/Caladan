cmake_minimum_required(VERSION 3.10)

project(Caladan)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(glfw3 3.3 REQUIRED)

find_package(Vulkan REQUIRED)

set(SOURCES
    main/Main.cpp
    renderer/Renderer.cpp
    renderer/window/Window.cpp
    renderer/graphicsPipeline/GraphicsPipeline.cpp
    renderer/graphicsPipeline/Device.cpp
    renderer/graphicsPipeline/SwapChain.cpp
)

# set(HEADERS
#     Renderer/Renderer.hpp
#     Renderer/Window/Window.hpp
# )

include_directories(
    ${CMAKE_SOURCE_DIR}/renderer
    ${CMAKE_SOURCE_DIR}/renderer/window
    ${CMAKE_SOURCE_DIR}/renderer/graphicsPipeline
)

add_executable(Caladan ${SOURCES} ${HEADERS})

target_link_libraries(Caladan glfw Vulkan::Vulkan)

add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/*
    COMMENT "Cleaning all build files"
)

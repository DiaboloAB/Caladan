cmake_minimum_required(VERSION 3.10)

project(Caladan)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(glfw3 3.3 REQUIRED)

find_package(Vulkan REQUIRED)

set(SOURCES
    src/main/Main.cpp
    src/render/Renderer.cpp
    src/render/RenderSystem.cpp
    src/render/window/Window.cpp
    src/render/graphicsPipeline/GraphicsPipeline.cpp
    src/render/graphicsPipeline/Device.cpp
    src/render/graphicsPipeline/SwapChain.cpp
    src/render/model/Model.cpp
    src/render/gameObject/GameObject.cpp
    src/render/camera/Camera.cpp
    src/core/App.cpp
    src/input/KeyboardMovementController.cpp
)

# set(HEADERS
#     Renderer/Renderer.hpp
#     Renderer/Window/Window.hpp
# )

include_directories(
    ${CMAKE_SOURCE_DIR}/src/render
    ${CMAKE_SOURCE_DIR}/src/render/window
    ${CMAKE_SOURCE_DIR}/src/render/graphicsPipeline
    ${CMAKE_SOURCE_DIR}/src/render/model
    ${CMAKE_SOURCE_DIR}/src/render/gameObject
    ${CMAKE_SOURCE_DIR}/src/render/camera
    ${CMAKE_SOURCE_DIR}/src/core
    #tmp
    ${CMAKE_SOURCE_DIR}/src/input

    #utils
    ${CMAKE_SOURCE_DIR}/src/utils

    ${CMAKE_SOURCE_DIR}/libs/tinyobjloader
)

add_executable(Caladan ${SOURCES} ${HEADERS})

target_link_libraries(Caladan glfw Vulkan::Vulkan)

add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/*
    COMMENT "Cleaning all build files"
)

cmake_minimum_required(VERSION 3.10)

project(Caladan)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(glfw3 3.3 REQUIRED)

find_package(Vulkan REQUIRED)

set(SOURCES
    main/Main.cpp
    render/Renderer.cpp
    render/RenderSystem.cpp
    render/window/Window.cpp
    render/graphicsPipeline/GraphicsPipeline.cpp
    render/graphicsPipeline/Device.cpp
    render/graphicsPipeline/SwapChain.cpp
    render/model/Model.cpp
    render/gameObject/GameObject.cpp
    render/camera/Camera.cpp
    core/App.cpp
    input/KeyboardMovementController.cpp
)

# set(HEADERS
#     Renderer/Renderer.hpp
#     Renderer/Window/Window.hpp
# )

include_directories(
    ${CMAKE_SOURCE_DIR}/render
    ${CMAKE_SOURCE_DIR}/render/window
    ${CMAKE_SOURCE_DIR}/render/graphicsPipeline
    ${CMAKE_SOURCE_DIR}/render/model
    ${CMAKE_SOURCE_DIR}/render/gameObject
    ${CMAKE_SOURCE_DIR}/render/camera
    ${CMAKE_SOURCE_DIR}/core
    #tmp
    ${CMAKE_SOURCE_DIR}/input
)

add_executable(Caladan ${SOURCES} ${HEADERS})

target_link_libraries(Caladan glfw Vulkan::Vulkan)

add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/*
    COMMENT "Cleaning all build files"
)

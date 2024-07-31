/**********************************************************************************
 * Project: Caladan
 * Description: Multi-platform 2D and 3D game engine
 * Author: Alexis BOITEL
 * Date, Location: 2024, Rennes
 **********************************************************************************/

#include <Camera.hpp>
#include <KeyboardMovementController.hpp>
#include <RenderSystem.hpp>
// glm
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
// std
#include <array>
#include <chrono>
#include <stdexcept>

#include "App.hpp"

using namespace Caladan;

App::App() { loadGameObjects(); }

App::~App() {}

void App::run()
{
    Render::RenderSystem simpleRenderSystem{_device, _renderer.getSwapChainRenderPass()};
    Render::Camera camera{};

    auto viewerObject = Render::GameObject::create();
    Input::KeyboardMovementController cameraController{};

    std::chrono::high_resolution_clock::time_point currentTime =
        std::chrono::high_resolution_clock::now();

    while (!_window.shouldClose())
    {
        glfwPollEvents();

        std::chrono::high_resolution_clock::time_point newTime =
            std::chrono::high_resolution_clock::now();
        // delta time in seconds
        // so 1/2 second will be 0.5f
        float deltaTime =
            std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime)
                .count();
        currentTime = newTime;

        // deltaTime = std::min(deltaTime, MAX_DELTA_TIME);

        cameraController.moveInPlaneXZ(_window.getWindow(), viewerObject, deltaTime);
        camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

        float aspect = _renderer.getAspectRatio();
        camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);
        if (auto commandBuffer = _renderer.beginFrame())
        {
            _renderer.beginSwapChainRenderPass(commandBuffer);
            simpleRenderSystem.renderGameObjects(commandBuffer, _gameObjects, camera);
            _renderer.endSwapChainRenderPass(commandBuffer);
            _renderer.endFrame();
        }
    }
    vkDeviceWaitIdle(_device.device());
}

// temporary helper function, creates a 1x1x1 cube centered at offset with an index buffer
std::unique_ptr<Render::Model> createCubeModel(Render::Device& device, glm::vec3 offset)
{
    Render::Model::Data modelData{};
    modelData.vertices = {
        // left face (white)
        {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
        {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
        {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
        {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},

        // right face (yellow)
        {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
        {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
        {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
        {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},

        // top face (orange, remember y axis points down)
        {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
        {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
        {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
        {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},

        // bottom face (red)
        {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
        {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
        {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
        {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},

        // nose face (blue)
        {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
        {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
        {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
        {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},

        // tail face (green)
        {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
        {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
        {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
        {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
    };
    for (auto& v : modelData.vertices)
    {
        v.position += offset;
    }

    modelData.indices = {0,  1,  2,  0,  3,  1,  4,  5,  6,  4,  7,  5,  8,  9,  10, 8,  11, 9,
                         12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21};

    return std::make_unique<Render::Model>(device, modelData);
}

void App::loadGameObjects()
{
    std::shared_ptr<Render::Model> cubeModel = createCubeModel(_device, {0.f, 0.f, 0.f});

    auto cube = Render::GameObject::create();
    cube.model = cubeModel;
    cube.transform.translation = {.0f, .0f, 2.5f};
    cube.transform.scale = {.5f, .5f, .5f};
    _gameObjects.push_back(std::move(cube));
}

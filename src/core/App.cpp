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

void App::loadGameObjects()
{
    std::shared_ptr<Render::Model> cubeModel =
        Render::Model::createModelFromFile(_device, "models/smooth_vase.obj");

    auto cube = Render::GameObject::create();
    cube.model = cubeModel;
    cube.transform.translation = {.0f, .0f, 2.5f};
    cube.transform.scale = {2, 2, 2};
    _gameObjects.push_back(std::move(cube));

    std::shared_ptr<Render::Model> cubeModel2 =
        Render::Model::createModelFromFile(_device, "models/flat_vase.obj");

    auto cube2 = Render::GameObject::create();
    cube2.model = cubeModel2;
    cube2.transform.translation = {.5f, .0f, 2.5f};
    cube2.transform.scale = {2, 2, 2};
    _gameObjects.push_back(std::move(cube2));
}

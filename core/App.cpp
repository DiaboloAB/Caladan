/**********************************************************************************
 * Project: Caladan
 * Description: Multi-platform 2D and 3D game engine
 * Author: Alexis BOITEL
 * Date, Location: 2024, Rennes
 **********************************************************************************/

#include <RenderSystem.hpp>
// glm
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
// std
#include <array>
#include <stdexcept>

#include "App.hpp"

using namespace Caladan;

App::App() { loadGameObjects(); }

App::~App() {}

void App::run()
{
    Render::RenderSystem simpleRenderSystem{_device, _renderer.getSwapChainRenderPass()};
    while (!_window.shouldClose())
    {
        glfwPollEvents();
        if (auto commandBuffer = _renderer.beginFrame())
        {
            _renderer.beginSwapChainRenderPass(commandBuffer);
            simpleRenderSystem.renderGameObjects(commandBuffer, _gameObjects);
            _renderer.endSwapChainRenderPass(commandBuffer);
            _renderer.endFrame();
        }
    }
    vkDeviceWaitIdle(_device.device());
}

void App::loadGameObjects()
{
    std::vector<Render::Model::Vertex> vertices = {
        {{0.0f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
    };
    auto model = std::make_shared<Render::Model>(_device, vertices);

    auto triangle1 = Render::GameObject::create();
    triangle1.model = model;
    triangle1.color = {1.0f, 0.0f, 0.0f};
    triangle1.transform2d.translation.x = 0.2f;
    triangle1.transform2d.scale = {2.0f, 1.0f};
    triangle1.transform2d.rotation = glm::half_pi<float>();

    _gameObjects.push_back(std::move(triangle1));
}

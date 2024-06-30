/**********************************************************************************
 * Project: Caladan
 * Description: Multi-platform 2D and 3D game engine
 * Author: Alexis BOITEL
 * Date, Location: 2024, Rennes
 **********************************************************************************/

#include "CaladanApp.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <stdexcept>

using namespace Caladan::Renderer;

struct SimplePushConstantData
{
    alignas(16) glm::mat2 transform{1.0f};
    alignas(16) glm::vec2 offset;
    alignas(16) glm::vec3 color;
};

CaladanApp::CaladanApp()
{
    loadGameObjects();
    createPipelineLayout();
    createPipeline();
}

CaladanApp::~CaladanApp() { vkDestroyPipelineLayout(_device.device(), _pipelineLayout, nullptr); }

void CaladanApp::run()
{
    while (!_window.shouldClose())
    {
        glfwPollEvents();
        if (auto commandBuffer = _renderer.beginFrame())
        {
            _renderer.beginSwapChainRenderPass(commandBuffer);
            renderGameObjects(commandBuffer);
            _renderer.endSwapChainRenderPass(commandBuffer);
            _renderer.endFrame();
        }
    }
    vkDeviceWaitIdle(_device.device());
}

void CaladanApp::createPipelineLayout()
{
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(SimplePushConstantData);

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

    if (vkCreatePipelineLayout(_device.device(), &pipelineLayoutInfo, nullptr, &_pipelineLayout) !=
        VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create pipeline layout");
    }
}

void CaladanApp::createPipeline()
{
    assert(_pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfigInfo{};
    GraphicsPipeline::defaultPipelineConfigInfo(pipelineConfigInfo);
    pipelineConfigInfo.renderPass = _renderer.getSwapChainRenderPass();
    pipelineConfigInfo.pipelineLayout = _pipelineLayout;
    _graphicsPipeline =
        std::make_unique<GraphicsPipeline>(_device, "shaders/simpleShader.vert.spv",
                                           "shaders/simpleShader.frag.spv", pipelineConfigInfo);
}

void CaladanApp::loadGameObjects()
{
    std::vector<Model::Vertex> vertices = {
        {{0.0f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
    };
    auto model = std::make_shared<Model>(_device, vertices);

    auto triangle1 = GameObject::create();
    triangle1.model = model;
    triangle1.color = {1.0f, 0.0f, 0.0f};
    triangle1.transform2d.translation.x = 0.2f;
    triangle1.transform2d.scale = {2.0f, 1.0f};
    triangle1.transform2d.rotation = glm::half_pi<float>();

    _gameObjects.push_back(std::move(triangle1));
}

void CaladanApp::renderGameObjects(VkCommandBuffer commandBuffer)
{
    _graphicsPipeline->bind(commandBuffer);

    for (auto &gameObject : _gameObjects)
    {
        gameObject.transform2d.rotation =
            glm::mod(gameObject.transform2d.rotation + 0.004f, glm::two_pi<float>());

        SimplePushConstantData push{};
        push.offset = gameObject.transform2d.translation;
        push.color = gameObject.color;
        push.transform = gameObject.transform2d.mat2();
        vkCmdPushConstants(commandBuffer, _pipelineLayout,
                           VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0,
                           sizeof(SimplePushConstantData), &push);
        gameObject.model->bind(commandBuffer);
        gameObject.model->draw(commandBuffer);
    }
}

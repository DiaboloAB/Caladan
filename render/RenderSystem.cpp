/**********************************************************************************
 * Project: Caladan
 * Description: Multi-platform 2D and 3D game engine
 * Author: Alexis BOITEL
 * Date, Location: 2024, Rennes
 **********************************************************************************/

#include "RenderSystem.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <stdexcept>

using namespace Caladan::Render;

struct SimplePushConstantData
{
    alignas(16) glm::mat2 transform{1.0f};
    alignas(16) glm::vec2 offset;
    alignas(16) glm::vec3 color;
};

RenderSystem::RenderSystem(Device &device, VkRenderPass renderPass) : _device(device)
{
    createPipelineLayout();
    createPipeline(renderPass);
}

RenderSystem::~RenderSystem()
{
    vkDestroyPipelineLayout(_device.device(), _pipelineLayout, nullptr);
}

void RenderSystem::createPipelineLayout()
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

void RenderSystem::createPipeline(VkRenderPass renderPass)
{
    assert(_pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfigInfo{};
    GraphicsPipeline::defaultPipelineConfigInfo(pipelineConfigInfo);
    pipelineConfigInfo.renderPass = renderPass;
    pipelineConfigInfo.pipelineLayout = _pipelineLayout;
    _graphicsPipeline =
        std::make_unique<GraphicsPipeline>(_device, "shaders/simpleShader.vert.spv",
                                           "shaders/simpleShader.frag.spv", pipelineConfigInfo);
}

void RenderSystem::renderGameObjects(VkCommandBuffer commandBuffer,
                                     const std::vector<GameObject> &gameObjects)
{
    _graphicsPipeline->bind(commandBuffer);

    for (auto &gameObject : gameObjects)
    {
        // gameObject.transform2d.rotation =
        //     glm::mod(gameObject.transform2d.rotation + 0.004f, glm::two_pi<float>());

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

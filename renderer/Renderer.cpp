/**********************************************************************************
 * Project: Caladan
 * Description: Multi-platform 2D and 3D game engine
 * Author: Alexis BOITEL
 * Date, Location: 2024, Rennes
 **********************************************************************************/

#include "Renderer.hpp"

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

Renderer::Renderer()
{
    loadGameObjects();
    createPipelineLayout();
    recreateSwapChain();
    createCommandBuffers();
}

Renderer::~Renderer() { vkDestroyPipelineLayout(_device.device(), _pipelineLayout, nullptr); }

void Renderer::run()
{
    while (!_window.shouldClose())
    {
        glfwPollEvents();
        drawFrame();
    }
    vkDeviceWaitIdle(_device.device());
}

void Renderer::createPipelineLayout()
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

void Renderer::createPipeline()
{
    assert(_swapChain != nullptr && "Cannot create pipeline before swap chain");
    assert(_pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfigInfo{};
    GraphicsPipeline::defaultPipelineConfigInfo(pipelineConfigInfo);
    pipelineConfigInfo.renderPass = _swapChain->getRenderPass();
    pipelineConfigInfo.pipelineLayout = _pipelineLayout;
    _graphicsPipeline =
        std::make_unique<GraphicsPipeline>(_device, "shaders/simpleShader.vert.spv",
                                           "shaders/simpleShader.frag.spv", pipelineConfigInfo);
}

void Renderer::recreateSwapChain()
{
    auto extent = _window.getExtent();
    while (extent.width == 0 || extent.height == 0)
    {
        extent = _window.getExtent();
        glfwWaitEvents();
    }
    vkDeviceWaitIdle(_device.device());
    if (_swapChain == nullptr)
    {
        _swapChain = std::make_unique<SwapChain>(_device, extent);
    }
    else
    {
        std::shared_ptr<SwapChain> oldSwapChain = std::move(_swapChain);
        _swapChain = std::make_unique<SwapChain>(_device, extent, oldSwapChain);
        if (_swapChain->imageCount() != _commandBuffers.size())
        {
            freeCommandBuffers();
            createCommandBuffers();
        }
    }
    createPipeline();
}

void Renderer::createCommandBuffers()
{
    _commandBuffers.resize(_swapChain->imageCount());
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = _device.getCommandPool();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = static_cast<uint32_t>(_commandBuffers.size());

    if (vkAllocateCommandBuffers(_device.device(), &allocInfo, _commandBuffers.data()) !=
        VK_SUCCESS)
    {
        throw std::runtime_error("Failed to allocate command buffers");
    }
}

void Renderer::freeCommandBuffers()
{
    vkFreeCommandBuffers(_device.device(), _device.getCommandPool(),
                         static_cast<uint32_t>(_commandBuffers.size()), _commandBuffers.data());
    _commandBuffers.clear();
}

void Renderer::recordCommandBuffer(int imageIndex)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(_commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to begin recording command buffer");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = _swapChain->getRenderPass();
    renderPassInfo.framebuffer = _swapChain->getFrameBuffer(imageIndex);
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = _swapChain->getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
    clearValues[1].depthStencil = {1.0f, 0};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(_commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(_swapChain->width());
    viewport.height = static_cast<float>(_swapChain->height());
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(_commandBuffers[imageIndex], 0, 1, &viewport);
    VkRect2D scissor{{0, 0}, _swapChain->getSwapChainExtent()};
    vkCmdSetScissor(_commandBuffers[imageIndex], 0, 1, &scissor);

    renderGameObjects(_commandBuffers[imageIndex]);

    vkCmdEndRenderPass(_commandBuffers[imageIndex]);
    if (vkEndCommandBuffer(_commandBuffers[imageIndex]) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to record command buffer");
    }
}

void Renderer::drawFrame()
{
    uint32_t imageIndex;
    auto result = _swapChain->acquireNextImage(&imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        recreateSwapChain();
        return;
    }
    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error("Failed to acquire next image");
    }

    recordCommandBuffer(imageIndex);
    result = _swapChain->submitCommandBuffers(&_commandBuffers[imageIndex], &imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
        _window.framebufferResized())
    {
        _window.resetFramebufferResized();
        recreateSwapChain();
        return;
    }
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to submit command buffer");
    }
}

void Renderer::loadGameObjects()
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

void Renderer::renderGameObjects(VkCommandBuffer commandBuffer)
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

/**********************************************************************************
 * Project: Caladan
 * Description: Multi-platform 2D and 3D game engine
 * Author: Alexis BOITEL
 * Date, Location: 2024, Rennes
 **********************************************************************************/

#include "Renderer.hpp"

// std
#include <array>
#include <stdexcept>

using namespace Caladan::Renderer;

Renderer::Renderer()
{
    loadModel();
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
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

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

    _graphicsPipeline->bind(_commandBuffers[imageIndex]);
    _model->bind(_commandBuffers[imageIndex]);
    _model->draw(_commandBuffers[imageIndex]);

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

void Renderer::loadModel()
{
    std::vector<Model::Vertex> vertices = {
        {{0.0f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
    };
    _model = std::make_unique<Model>(_device, vertices);
}
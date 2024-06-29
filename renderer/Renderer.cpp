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
    createPipelineLayout();
    createPipeline();
    createCommandBuffers();
}

Renderer::~Renderer() { vkDestroyPipelineLayout(_device.device(), _pipelineLayout, nullptr); }

void Renderer::run()
{
    while (!_window.ShouldClose())
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
    auto pipelineConfigInfo =
        GraphicsPipeline::defaultPipelineConfigInfo(_swapChain.width(), _swapChain.height());
    pipelineConfigInfo.renderPass = _swapChain.getRenderPass();
    pipelineConfigInfo.pipelineLayout = _pipelineLayout;
    _graphicsPipeline =
        std::make_unique<GraphicsPipeline>(_device, "shaders/simpleShader.vert.spv",
                                           "shaders/simpleShader.frag.spv", pipelineConfigInfo);
}

void Renderer::createCommandBuffers()
{
    _commandBuffers.resize(_swapChain.imageCount());
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

    for (int i = 0; i < _commandBuffers.size(); i++)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(_commandBuffers[i], &beginInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to begin recording command buffer");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = _swapChain.getRenderPass();
        renderPassInfo.framebuffer = _swapChain.getFrameBuffer(i);
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = _swapChain.getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(_commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        _graphicsPipeline->bind(_commandBuffers[i]);
        vkCmdDraw(_commandBuffers[i], 3, 1, 0, 0);

        vkCmdEndRenderPass(_commandBuffers[i]);
        if (vkEndCommandBuffer(_commandBuffers[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to record command buffer");
        }
    }
}

void Renderer::drawFrame()
{
    uint32_t imageIndex;
    auto result = _swapChain.acquireNextImage(&imageIndex);
    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error("Failed to acquire next image");
    }

    result = _swapChain.submitCommandBuffers(&_commandBuffers[imageIndex], &imageIndex);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to submit command buffer");
    }
}

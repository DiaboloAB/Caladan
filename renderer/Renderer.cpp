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

Renderer::Renderer(Window &window, Device &device) : _window(window), _device(device)
{
    recreateSwapChain();
    createCommandBuffers();
}

Renderer::~Renderer() { freeCommandBuffers(); }

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

VkCommandBuffer Renderer::beginFrame()
{
    assert(!_isFrameStarted && "Can't call beginFrame while frame is in progress");
    auto result = _swapChain->acquireNextImage(&_currentImageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        recreateSwapChain();
        return nullptr;
    }
    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error("Failed to acquire next image");
    }
    _isFrameStarted = true;
    auto commandBuffer = _commandBuffers[_currentImageIndex];
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to begin recording command buffer");
    }
    return commandBuffer;
}
void Renderer::endFrame()
{
    assert(_isFrameStarted && "Can't call endFrame while frame is not in progress");
    auto commandBuffer = _commandBuffers[_currentImageIndex];
    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to record command buffer");
    }
    auto result = _swapChain->submitCommandBuffers(&commandBuffer, &_currentImageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
        _window.framebufferResized())
    {
        _window.resetFramebufferResized();
        recreateSwapChain();
    }
    else if (result != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to present swap chain image");
    }
    _isFrameStarted = false;
}
void Renderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer)
{
    assert(_isFrameStarted && "Can't begin render pass when frame is not in progress");
    assert(commandBuffer == getCurrentCommandBuffer() &&
           "Can't begin render pass on command buffer from a different frame");
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = _swapChain->getRenderPass();
    renderPassInfo.framebuffer = _swapChain->getFrameBuffer(_currentImageIndex);
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = _swapChain->getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
    clearValues[1].depthStencil = {1.0f, 0};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(_swapChain->width());
    viewport.height = static_cast<float>(_swapChain->height());
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    VkRect2D scissor{{0, 0}, _swapChain->getSwapChainExtent()};
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}
void Renderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer)
{
    assert(_isFrameStarted && "Can't end render pass when frame is not in progress");
    assert(commandBuffer == getCurrentCommandBuffer() &&
           "Can't end render pass on command buffer from a different frame");

    vkCmdEndRenderPass(commandBuffer);
}
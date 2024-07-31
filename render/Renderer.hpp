/**********************************************************************************
 * Project: Caladan
 * Description: Multi-platform 2D and 3D game engine
 * Author: Alexis BOITEL
 * Date, Location: 2024, Rennes
 **********************************************************************************/

#ifndef RENDERER_HPP_
#define RENDERER_HPP_

#include <Device.hpp>
#include <SwapChain.hpp>
#include <Window.hpp>

// std
#include <cassert>
#include <memory>
#include <vector>

namespace Caladan::Render
{
class Renderer
{
   public:
    Renderer(Window& window, Device& device);
    ~Renderer();

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    // getters
    VkRenderPass getSwapChainRenderPass() const { return _swapChain->getRenderPass(); }
    float getAspectRatio() const { return _swapChain->extentAspectRatio(); }
    bool isFrameStarted() const { return _isFrameStarted; }
    VkCommandBuffer getCurrentCommandBuffer() const
    {
        assert(_isFrameStarted && "Cannot get command buffer when frame is not started.");
        return _commandBuffers[_currentFrameIndex];
    }
    int getFrameIndex() const
    {
        assert(_isFrameStarted && "Cannot get frame when frame is not started.");
        return _currentFrameIndex;
    }

    VkCommandBuffer beginFrame();
    void endFrame();
    void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
    void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

   protected:
   private:
    uint32_t _currentImageIndex{0};
    int _currentFrameIndex{0};
    bool _isFrameStarted{false};

    void createCommandBuffers();
    void freeCommandBuffers();
    void recreateSwapChain();

    Window& _window;
    Device& _device;

    std::unique_ptr<SwapChain> _swapChain;

    std::vector<VkCommandBuffer> _commandBuffers;
};
}  // namespace Caladan::Render

#endif /* !RENDERER_HPP_ */

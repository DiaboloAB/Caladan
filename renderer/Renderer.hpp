/**********************************************************************************
 * Project: Caladan
 * Description: Multi-platform 2D and 3D game engine
 * Author: Alexis BOITEL
 * Date, Location: 2024, Rennes
 **********************************************************************************/

#ifndef RENDERER_HPP_
#define RENDERER_HPP_

#include <Device.hpp>
#include <GraphicsPipeline.hpp>
#include <SwapChain.hpp>
#include <Window.hpp>

// std
#include <memory>
#include <vector>

namespace Caladan::Renderer
{
class Renderer
{
   public:
    Renderer();
    ~Renderer();

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    void run();

   protected:
   private:
    void createPipelineLayout();
    void createPipeline();
    void createCommandBuffers();
    void drawFrame();

    Window _window{WIDTH, HEIGHT, "Caladan"};
    Device _device{_window};
    SwapChain _swapChain{_device, _window.GetExtent()};
    std::unique_ptr<GraphicsPipeline> _graphicsPipeline{nullptr};
    //  GraphicsPipeline _graphicsPipeline{_device, "shaders/simpleShader.vert.spv",
    //                                     "shaders/simpleShader.frag.spv",
    //                                     GraphicsPipeline::defaultPipelineConfigInfo(WIDTH,
    //                                     HEIGHT)};
    VkPipelineLayout _pipelineLayout;
    std::vector<VkCommandBuffer> _commandBuffers;
};
}  // namespace Caladan::Renderer

#endif /* !RENDERER_HPP_ */

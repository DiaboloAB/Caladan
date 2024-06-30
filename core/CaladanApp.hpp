/**********************************************************************************
 * Project: Caladan
 * Description: Multi-platform 2D and 3D game engine
 * Author: Alexis BOITEL
 * Date, Location: 2024, Rennes
 **********************************************************************************/

#ifndef CALADANAPP_HPP_
#define CALADANAPP_HPP_

#include <Device.hpp>
#include <GameObject.hpp>
#include <GraphicsPipeline.hpp>
#include <Renderer.hpp>
#include <Window.hpp>

// std
#include <cassert>
#include <memory>
#include <vector>

namespace Caladan::Renderer
{
class CaladanApp
{
   public:
    CaladanApp();
    ~CaladanApp();

    CaladanApp(const CaladanApp&) = delete;
    CaladanApp& operator=(const CaladanApp&) = delete;

    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    void run();

   protected:
   private:
    void loadGameObjects();
    void createPipelineLayout();
    void createPipeline();
    void renderGameObjects(VkCommandBuffer commandBuffer);

    Window _window{WIDTH, HEIGHT, "Caladan"};
    Device _device{_window};
    Renderer _renderer{_window, _device};

    std::unique_ptr<GraphicsPipeline> _graphicsPipeline{nullptr};
    VkPipelineLayout _pipelineLayout;
    std::vector<GameObject> _gameObjects;
};
}  // namespace Caladan::Renderer

#endif /* !CALADANAPP_HPP_ */

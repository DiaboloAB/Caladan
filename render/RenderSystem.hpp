/**********************************************************************************
 * Project: Caladan
 * Description: Multi-platform 2D and 3D game engine
 * Author: Alexis BOITEL
 * Date, Location: 2024, Rennes
 **********************************************************************************/

#ifndef RENDERSYSTEM_HPP_
#define RENDERSYSTEM_HPP_

#include <Camera.hpp>
#include <Device.hpp>
#include <GameObject.hpp>
#include <GraphicsPipeline.hpp>

// std
#include <cassert>
#include <memory>
#include <vector>

namespace Caladan::Render
{
class RenderSystem
{
   public:
    RenderSystem(Device& device, VkRenderPass renderPass);
    ~RenderSystem();

    RenderSystem(const RenderSystem&) = delete;
    RenderSystem& operator=(const RenderSystem&) = delete;

    void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject>& gameObjects,
                           const Camera& camera);

   protected:
   private:
    void createPipelineLayout();
    void createPipeline(VkRenderPass renderPass);

    Device& _device;

    std::unique_ptr<Render::GraphicsPipeline> _graphicsPipeline{nullptr};
    VkPipelineLayout _pipelineLayout;
};
}  // namespace Caladan::Render

#endif /* !RENDERSYSTEM_HPP_ */

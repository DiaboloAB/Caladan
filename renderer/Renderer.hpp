/**********************************************************************************
 * Project: Caladan
 * Description: Multi-platform 2D and 3D game engine
 * Author: Alexis BOITEL
 * Date, Location: 2024, Rennes
 **********************************************************************************/

#ifndef RENDERER_HPP_
#define RENDERER_HPP_

#include <GraphicsPipeline.hpp>
#include <Window.hpp>

namespace Caladan::Renderer
{
class Renderer
{
   public:
    Renderer();
    ~Renderer();

    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    void Run();

   protected:
   private:
    Window _window{WIDTH, HEIGHT, "Caladan"};
    GraphicsPipeline _graphicsPipeline{"shaders/simpleShader.vert.spv",
                                       "shaders/simpleShader.frag.spv"};
};
}  // namespace Caladan::Renderer

#endif /* !RENDERER_HPP_ */

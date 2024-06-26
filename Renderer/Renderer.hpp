/**********************************************************************************
 * Project: Caladan
 * Description: Multi-platform 2D and 3D game engine
 * Author: Alexis BOITEL
 * Date, Location: 2024, Rennes
 **********************************************************************************/

#ifndef RENDERER_HPP_
#define RENDERER_HPP_

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

    void Run(){};

   protected:
   private:
    Window _window{WIDTH, HEIGHT, "Caladan"};
};
}  // namespace Caladan::Renderer

#endif /* !RENDERER_HPP_ */

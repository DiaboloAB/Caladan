/**********************************************************************************
 * Project: Caladan
 * Description: Multi-platform 2D and 3D game engine
 * Author: Alexis BOITEL
 * Date, Location: 2024, Rennes
 **********************************************************************************/

#ifndef APP_HPP_
#define APP_HPP_

#include <Device.hpp>
#include <GameObject.hpp>
#include <Renderer.hpp>
#include <Window.hpp>

// std
#include <cassert>
#include <memory>
#include <vector>

namespace Caladan
{
class App
{
   public:
    App();
    ~App();

    App(const App&) = delete;
    App& operator=(const App&) = delete;

    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    void run();

   protected:
   private:
    void loadGameObjects();

    Render::Window _window{WIDTH, HEIGHT, "Caladan"};
    Render::Device _device{_window};
    Render::Renderer               _renderer{_window, _device};

    std::vector<Render::GameObject> _gameObjects;
};
}  // namespace Caladan

#endif /* !APP_HPP_ */

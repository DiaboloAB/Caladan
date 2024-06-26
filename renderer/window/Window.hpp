/**********************************************************************************
 * Project: Caladan
 * Description: Multi-platform 2D and 3D game engine
 * Author: Alexis BOITEL
 * Date, Location: 2024, Rennes
 **********************************************************************************/

#ifndef WINDOW_H_
#define WINDOW_H_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// std
#include <string>

namespace Caladan::Renderer
{
class Window
{
   public:
    Window(int width, int height, std::string name);
    ~Window();
    void CreateWindow();
    void DestroyWindow();

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    bool ShouldClose() const { return glfwWindowShouldClose(_window); }

    // Getters
    GLFWwindow *GetWindow() const { return _window; }

    // Setters

   protected:
   private:
    int _width;
    int _height;
    std::string _name;
    GLFWwindow *_window;
};
}  // namespace Caladan::Renderer

#endif /* !WINDOW_H_ */

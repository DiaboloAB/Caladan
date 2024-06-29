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

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    void createWindow();
    void destroyWindow();
    bool shouldClose() const { return glfwWindowShouldClose(_window); }
    void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

    // Getters
    GLFWwindow *getWindow() const { return _window; }
    VkExtent2D getExtent() const
    {
        return {static_cast<uint32_t>(_width), static_cast<uint32_t>(_height)};
    }
    bool framebufferResized() const { return _framebufferResized; }
    void resetFramebufferResized() { _framebufferResized = false; }

    // Setters

   protected:
   private:
    static void framebufferResizeCallback(GLFWwindow *window, int width, int height);

    int _width;
    int _height;
    bool _framebufferResized = false;

    std::string _name;
    GLFWwindow *_window;
};
}  // namespace Caladan::Renderer

#endif /* !WINDOW_H_ */

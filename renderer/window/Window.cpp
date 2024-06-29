/**********************************************************************************
 * Project: Caladan
 * Description: Multi-platform 2D and 3D game engine
 * Author: Alexis BOITEL
 * Date, Location: 2024, Rennes
 **********************************************************************************/

#include "Window.hpp"

// std
#include <iostream>
#include <stdexcept>

using namespace Caladan::Renderer;

Window::Window(int width, int height, std::string name)
    : _width(width), _height(height), _name(name)
{
    createWindow();
}

Window::~Window() { destroyWindow(); }

void Window::createWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    uint32_t glfwExtensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &glfwExtensionCount, nullptr);

    _window = glfwCreateWindow(_width, _height, _name.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(_window, this);
    glfwSetFramebufferSizeCallback(_window, framebufferResizeCallback);
}

void Window::destroyWindow()
{
    glfwDestroyWindow(_window);
    glfwTerminate();
}

void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface)
{
    if (glfwCreateWindowSurface(instance, _window, nullptr, surface) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create window surface!");
    }
}

void Window::framebufferResizeCallback(GLFWwindow *window, int width, int height)
{
    auto app = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
    app->_framebufferResized = true;
    app->_width = width;
    app->_height = height;
}

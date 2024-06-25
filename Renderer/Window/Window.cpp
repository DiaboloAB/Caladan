/*
** EPITECH PROJECT, 2024
** Caladan
** File description:
** Window
*/

#include "Window.h"

using namespace Caladan::Renderer;

Window::Window()
{
    CreateWindow();
}

Window::~Window()
{
    DestroyWindow();
}

void Window::CreateWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    _window = glfwCreateWindow(800, 600, "Vulkan Window", nullptr, nullptr);
}

void Window::DestroyWindow()
{
    glfwDestroyWindow(_window);
    glfwTerminate();
}
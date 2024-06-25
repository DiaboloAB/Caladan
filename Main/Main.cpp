/*
** EPITECH PROJECT, 2024
** Caladan
** File description:
** Main
*/


#include "GLFW/glfw3.h"

#include <Window.h>

int main()
{
    Caladan::Renderer::Window window;
    while (!glfwWindowShouldClose(window.GetWindow()))
    {
        glfwPollEvents();
    }
    // window.CreateWindow();

    // GLFWwindow* window;
    // glfwInit();
    // glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    // glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // window = glfwCreateWindow(800, 600, "Vulkan Window", nullptr, nullptr);
    // if (!window)
    // {
    //     glfwTerminate();
    //     return 64;
    // }
    // while (!glfwWindowShouldClose(window))
    // {
    //     // glClear(GL_COLOR_BUFFER_BIT);
    //     // glfwSwapBuffers(window);
    //     // glfwPollEvents();
    // }
    // glfwTerminate();
    // return 0;
}
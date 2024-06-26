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
    Caladan::Renderer::Renderer renderer;
    try {

    renderer.Run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
    return 0;
}
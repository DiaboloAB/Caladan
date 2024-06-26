/**********************************************************************************
 * Project: Caladan
 * Description: Multi-platform 2D and 3D game engine
 * Author: Alexis BOITEL
 * Date, Location: 2024, Rennes
 **********************************************************************************/

#include <Renderer.hpp>

#include "GLFW/glfw3.h"

// std
#include <iostream>
#include <stdexcept>

int main()
{
    Caladan::Renderer::Renderer renderer;
    try
    {
        renderer.Run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
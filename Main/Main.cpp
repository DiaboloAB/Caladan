/**********************************************************************************
 * Project: Caladan
 * Description: Multi-platform 2D and 3D game engine
 * Author: Alexis BOITEL
 * Date, Location: 2024, Rennes
 **********************************************************************************/

#include <Window.h>

#include "GLFW/glfw3.h"

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
        return 84;
    }
    return 0;
}
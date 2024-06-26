/**********************************************************************************
 * Project: Caladan
 * Description: Multi-platform 2D and 3D game engine
 * Author: Alexis BOITEL
 * Date, Location: 2024, Rennes
 **********************************************************************************/

#include "Renderer.hpp"

using namespace Caladan::Renderer;

Renderer::Renderer() {}

Renderer::~Renderer() {}

void Renderer::Run() {
    while (!_window.ShouldClose()) {
        glfwPollEvents();
    }
}

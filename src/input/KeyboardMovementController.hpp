/**********************************************************************************
 * Project: Caladan
 * Description: Multi-platform 2D and 3D game engine
 * Author: Alexis BOITEL
 * Date, Location: 2024, Rennes
 **********************************************************************************/

#ifndef KEYBOARDMOVEMENTCONTROLLER_H
#define KEYBOARDMOVEMENTCONTROLLER_H

#include <GameObject.hpp>
#include <Window.hpp>

// std

namespace Caladan::Input
{
// !!! temporary class !!!! to be removed
class KeyboardMovementController
{
   public:
    KeyboardMovementController();
    ~KeyboardMovementController();

    struct KeyBinding
    {
        int left = GLFW_KEY_A;
        int right = GLFW_KEY_D;
        int forward = GLFW_KEY_W;
        int back = GLFW_KEY_S;
        int up = GLFW_KEY_SPACE;
        int down = GLFW_KEY_LEFT_SHIFT;
        int lookLeft = GLFW_KEY_LEFT;
        int lookRight = GLFW_KEY_RIGHT;
        int lookUp = GLFW_KEY_UP;
        int lookDown = GLFW_KEY_DOWN;
    };

    void moveInPlaneXZ(GLFWwindow* window, Caladan::Render::GameObject& gameObject,
                       float deltaTime);

    KeyBinding _keys{};
    float _movementSpeed = 3.0f;
    float _rotationSpeed = 1.5f;

    // Getters

    // Setters

   private:
    // Member variables
};

}  // namespace Caladan::Input

#endif  // KEYBOARDMOVEMENTCONTROLLER_H
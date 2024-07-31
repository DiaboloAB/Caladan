/**********************************************************************************
 * Project: Caladan
 * Description: Multi-platform 2D and 3D game engine
 * Author: Alexis BOITEL
 * Date, Location: 2024, Rennes
 **********************************************************************************/

#include "KeyboardMovementController.hpp"

using namespace Caladan::Input;

KeyboardMovementController::KeyboardMovementController()
{
    // Constructor implementation
}

KeyboardMovementController::~KeyboardMovementController()
{
    // Destructor implementation
}

void KeyboardMovementController::moveInPlaneXZ(GLFWwindow* window,
                                               Caladan::Render::GameObject& gameObject,
                                               float deltaTime)
{
    glm::vec3 rotate{0};
    if (glfwGetKey(window, _keys.lookRight) == GLFW_PRESS) rotate.y += 1.f;
    if (glfwGetKey(window, _keys.lookLeft) == GLFW_PRESS) rotate.y -= 1.f;
    if (glfwGetKey(window, _keys.lookUp) == GLFW_PRESS) rotate.x += 1.f;
    if (glfwGetKey(window, _keys.lookDown) == GLFW_PRESS) rotate.x -= 1.f;

    if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon())
    {
        gameObject.transform.rotation += _rotationSpeed * deltaTime * glm::normalize(rotate);
    }

    // limit pitch values between about +/- 85ish degrees
    gameObject.transform.rotation.x = glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
    gameObject.transform.rotation.y =
        glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>());

    float yaw = gameObject.transform.rotation.y;
    const glm::vec3 forwardDir{sin(yaw), 0.f, cos(yaw)};
    const glm::vec3 rightDir{forwardDir.z, 0.f, -forwardDir.x};
    const glm::vec3 upDir{0.f, -1.f, 0.f};

    glm::vec3 moveDir{0.f};
    if (glfwGetKey(window, _keys.forward) == GLFW_PRESS) moveDir += forwardDir;
    if (glfwGetKey(window, _keys.back) == GLFW_PRESS) moveDir -= forwardDir;
    if (glfwGetKey(window, _keys.right) == GLFW_PRESS) moveDir += rightDir;
    if (glfwGetKey(window, _keys.left) == GLFW_PRESS) moveDir -= rightDir;
    if (glfwGetKey(window, _keys.up) == GLFW_PRESS) moveDir += upDir;
    if (glfwGetKey(window, _keys.down) == GLFW_PRESS) moveDir -= upDir;

    if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon())
    {
        gameObject.transform.translation += _movementSpeed * deltaTime * glm::normalize(moveDir);
    }
}
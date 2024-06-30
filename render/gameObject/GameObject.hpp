/**********************************************************************************
 * Project: Caladan
 * Description: Multi-platform 2D and 3D game engine
 * Author: Alexis BOITEL
 * Date, Location: 2024, Rennes
 **********************************************************************************/

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <Model.hpp>

// std
#include <memory>

namespace Caladan::Render
{

struct Transform2dComponent
{
    glm::vec2 translation{};  // (position offset)
    glm::vec2 scale{1.f, 1.f};
    float rotation;

    glm::mat2 mat2() const
    {
        const float s = glm::sin(rotation);
        const float c = glm::cos(rotation);
        glm::mat2 rotMatrix{{c, s}, {-s, c}};

        glm::mat2 scaleMat{{scale.x, .0f}, {.0f, scale.y}};
        return rotMatrix * scaleMat;
    }
};
class GameObject
{
   public:
    ~GameObject();

    // GameObject(const GameObject&) = delete;
    // GameObject& operator=(const GameObject&) = delete;
    // GameObject(GameObject&&) = delete;
    // GameObject& operator=(GameObject&&) = delete;

    using id_t = unsigned int;

    std::shared_ptr<Model> model{};
    glm::vec3 color{};
    Transform2dComponent transform2d{};

    static GameObject create()
    {
        static id_t id = 0;
        return GameObject(id++);
    }

    // Getters
    id_t id() const { return _id; }

    // Setters

   private:
    GameObject(id_t id) : _id(id) {}

    // Member variables
    id_t _id;
};

}  // namespace Caladan::Render

#endif  // GAMEOBJECT_H
/**********************************************************************************
 * Project: Caladan
 * Description: Multi-platform 2D and 3D game engine
 * Author: Alexis BOITEL
 * Date, Location: 2024, Rennes
 **********************************************************************************/

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <Model.hpp>
#include <glm/gtc/matrix_transform.hpp>

// std
#include <memory>

namespace Caladan::Render
{

struct TransformComponent
{
    glm::vec3 translation{};  // (position offset)
    glm::vec3 scale{1.f, 1.f, 1.f};
    glm::vec3 rotation{};

    glm::mat4 mat4()
    {
        const float c3 = glm::cos(rotation.z);
        const float s3 = glm::sin(rotation.z);
        const float c2 = glm::cos(rotation.x);
        const float s2 = glm::sin(rotation.x);
        const float c1 = glm::cos(rotation.y);
        const float s1 = glm::sin(rotation.y);
        return glm::mat4{{
                             scale.x * (c1 * c3 + s1 * s2 * s3),
                             scale.x * (c2 * s3),
                             scale.x * (c1 * s2 * s3 - c3 * s1),
                             0.0f,
                         },
                         {
                             scale.y * (c3 * s1 * s2 - c1 * s3),
                             scale.y * (c2 * c3),
                             scale.y * (c1 * c3 * s2 + s1 * s3),
                             0.0f,
                         },
                         {
                             scale.z * (c2 * s1),
                             scale.z * (-s2),
                             scale.z * (c1 * c2),
                             0.0f,
                         },
                         {translation.x, translation.y, translation.z, 1.0f}};
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
    TransformComponent transform{};

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
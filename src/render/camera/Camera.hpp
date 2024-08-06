/**********************************************************************************
 * Project: Caladan
 * Description: Multi-platform 2D and 3D game engine
 * Author: Alexis BOITEL
 * Date, Location: 2024, Rennes
 **********************************************************************************/

#ifndef CAMERA_H
#define CAMERA_H

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// std

namespace Caladan::Render
{

class Camera
{
   public:
    Camera();
    ~Camera();

    void setOrthographicProjection(float left, float right, float top, float bottom, float near,
                                   float far);
    void setPerspectiveProjection(float fovy, float aspect, float near, float far);

    void setViewDirection(glm::vec3 position, glm::vec3 direction,
                          glm::vec3 up = glm::vec3{0.f, -1.f, 0.f});
    void setViewTarget(glm::vec3 position, glm::vec3 target,
                       glm::vec3 up = glm::vec3{0.f, -1.f, 0.f});
    void setViewYXZ(glm::vec3 position, glm::vec3 rotation);

    const glm::mat4& getProjection() const { return _projection; }
    const glm::mat4& getView() const { return _view; }

   private:
    glm::mat4 _projection{1.f};
    glm::mat4 _view{1.f};
};

}  // namespace Caladan::Render

#endif  // CAMERA_H
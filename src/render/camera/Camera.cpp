/**********************************************************************************
 * Project: Caladan
 * Description: Multi-platform 2D and 3D game engine
 * Author: Alexis BOITEL
 * Date, Location: 2024, Rennes
 **********************************************************************************/

#include "Camera.hpp"

// std
#include <cassert>
#include <cstring>
#include <iostream>

using namespace Caladan::Render;

Camera::Camera()
{
    // Constructor implementation
}

Camera::~Camera()
{
    // Destructor implementation
}

void Camera::setOrthographicProjection(float left, float right, float top, float bottom, float near,
                                       float far)
{
    _projection = glm::mat4{1.0f};
    _projection[0][0] = 2.f / (right - left);
    _projection[1][1] = 2.f / (bottom - top);
    _projection[2][2] = 1.f / (far - near);
    _projection[3][0] = -(right + left) / (right - left);
    _projection[3][1] = -(bottom + top) / (bottom - top);
    _projection[3][2] = -near / (far - near);
}

void Camera::setPerspectiveProjection(float fovy, float aspect, float near, float far)
{
    assert(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
    const float tanHalfFovy = tan(fovy / 2.f);
    _projection = glm::mat4{0.0f};
    _projection[0][0] = 1.f / (aspect * tanHalfFovy);
    _projection[1][1] = 1.f / (tanHalfFovy);
    _projection[2][2] = far / (far - near);
    _projection[2][3] = 1.f;
    _projection[3][2] = -(far * near) / (far - near);
}

void Camera::setViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up)
{
    const glm::vec3 w{glm::normalize(direction)};
    const glm::vec3 u{glm::normalize(glm::cross(w, up))};
    const glm::vec3 v{glm::cross(w, u)};

    _view = glm::mat4{1.f};
    _view[0][0] = u.x;
    _view[1][0] = u.y;
    _view[2][0] = u.z;
    _view[0][1] = v.x;
    _view[1][1] = v.y;
    _view[2][1] = v.z;
    _view[0][2] = w.x;
    _view[1][2] = w.y;
    _view[2][2] = w.z;
    _view[3][0] = -glm::dot(u, position);
    _view[3][1] = -glm::dot(v, position);
    _view[3][2] = -glm::dot(w, position);
}

void Camera::setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up)
{
    if (position == target)
    {
        throw std::runtime_error("Camera position and target cannot be the same");
    }
    setViewDirection(position, target - position, up);
}

void Camera::setViewYXZ(glm::vec3 position, glm::vec3 rotation)
{
    const float c3 = glm::cos(rotation.z);
    const float s3 = glm::sin(rotation.z);
    const float c2 = glm::cos(rotation.x);
    const float s2 = glm::sin(rotation.x);
    const float c1 = glm::cos(rotation.y);
    const float s1 = glm::sin(rotation.y);
    const glm::vec3 u{(c1 * c3 + s1 * s2 * s3), (c2 * s3), (c1 * s2 * s3 - c3 * s1)};
    const glm::vec3 v{(c3 * s1 * s2 - c1 * s3), (c2 * c3), (c1 * c3 * s2 + s1 * s3)};
    const glm::vec3 w{(c2 * s1), (-s2), (c1 * c2)};
    _view = glm::mat4{1.f};
    _view[0][0] = u.x;
    _view[1][0] = u.y;
    _view[2][0] = u.z;
    _view[0][1] = v.x;
    _view[1][1] = v.y;
    _view[2][1] = v.z;
    _view[0][2] = w.x;
    _view[1][2] = w.y;
    _view[2][2] = w.z;
    _view[3][0] = -glm::dot(u, position);
    _view[3][1] = -glm::dot(v, position);
    _view[3][2] = -glm::dot(w, position);
}
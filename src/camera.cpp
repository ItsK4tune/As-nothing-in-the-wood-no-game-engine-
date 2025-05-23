#include "util/camera.h"

Camera::Camera(const glm::vec3 &position,
               const glm::vec3 &target,
               const glm::vec3 &up,
               const float &fovDegrees,
               const float &aspectRatio,
               const float &nearPlane,
               const float &farPlane)
    : m_position(position),
      m_target(target),
      m_up(up),
      m_fovDegrees(fovDegrees),
      m_aspectRatio(aspectRatio),
      m_nearPlane(nearPlane),
      m_farPlane(farPlane),
      m_projectionType(ProjectionType::Perspective) {}

void Camera::setPosition(const glm::vec3 &position) { m_position = position; }
void Camera::setTarget(const glm::vec3 &target) { m_target = target; }
void Camera::setAspectRatio(float aspect) { m_aspectRatio = aspect; }
void Camera::setProjectionType(ProjectionType type) { m_projectionType = type; }
void Camera::setOrthoBounds(float left, float right, float bottom, float top)
{
    m_left = left;
    m_right = right;
    m_bottom = bottom;
    m_top = top;
}
// void Camera::updateFromWindowSize(int width, int height)
// {
//     if (height == 0)
//         height = 1;
//     m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);

//     if (m_projectionType == ProjectionType::Orthographic)
//     {
//         float viewHeight = m_top - m_bottom;
//         float viewWidth = viewHeight * m_aspectRatio;

//         float centerX = (m_left + m_right) / 2.0f;
//         m_left = centerX - viewWidth / 2.0f;
//         m_right = centerX + viewWidth / 2.0f;
//     }
// }
void Camera::move(const glm::vec3 &direction, float amount)
{
    m_position += direction * amount;
    m_target += direction * amount;
}

glm::mat4 Camera::getViewMatrix() const { return glm::lookAt(m_position, m_target, m_up); }
glm::mat4 Camera::getProjectionMatrix() const
{
    if (m_projectionType == ProjectionType::Perspective)
    {
        return glm::perspective(glm::radians(m_fovDegrees), m_aspectRatio, m_nearPlane, m_farPlane);
    }
    else
    {
        return glm::ortho(m_left, m_right, m_bottom, m_top, m_nearPlane, m_farPlane);
    }
}
glm::vec3 Camera::getPosition() const { return m_position; }
glm::vec3 Camera::getTarget() const { return m_target; }
glm::vec3 Camera::getUp() const { return m_up; }
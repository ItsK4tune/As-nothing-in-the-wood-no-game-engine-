#include "util/camera.h"
#include <iostream>

Camera::Camera(const glm::vec3 &position,
               const glm::vec3 &direction,
               const glm::vec3 &relativeOffset,
               const glm::vec3 &up,
               float fovDegrees,
               float aspectRatio,
               float nearPlane,
               float farPlane)
    : m_position(position),
      m_direction(glm::normalize(direction)),
      m_relativeOffset(relativeOffset),
      m_up(glm::normalize(up)),
      m_fovDegrees(fovDegrees),
      m_aspectRatio(aspectRatio),
      m_nearPlane(nearPlane),
      m_farPlane(farPlane),
      m_projectionType(ProjectionType::Perspective) {}

void Camera::setPosition(const glm::vec3 &position) { m_position = position; }
void Camera::setDirection(const glm::vec3 &direction) { m_direction = glm::normalize(direction); }
void Camera::setRelativeOffset(const glm::vec3 &offset) { m_relativeOffset = offset; }
void Camera::setUp(const glm::vec3 &up) { m_up = glm::normalize(up); }
void Camera::setPitchYaw(float pitch, float yaw)
{
    this->pitch = pitch;
    this->yaw = yaw;
}
void Camera::setAspectRatio(float aspect) { m_aspectRatio = aspect; }
void Camera::setProjectionType(ProjectionType type) { m_projectionType = type; }
void Camera::setOrthoBounds(float left, float right, float bottom, float top)
{
    m_left = left;
    m_right = right;
    m_bottom = bottom;
    m_top = top;
}
void Camera::updateFromPlayer(const glm::vec3 &playerPosition, const glm::vec3 &playerDirection)
{
    glm::vec3 behind = -glm::normalize(playerDirection);
    glm::vec3 right = glm::normalize(glm::cross(playerDirection, m_up));
    glm::vec3 up = m_up;

    glm::vec3 finalPosition = playerPosition + right * m_relativeOffset.x + up * m_relativeOffset.y + behind * m_relativeOffset.z;

    m_position = finalPosition;

    // glm::vec3 look = playerPosition - m_position;
    // m_direction = glm::length(look) < 0.0001f
    //                   ? glm::normalize(playerDirection)
    //                   : glm::normalize(look);
    m_direction = glm::normalize(playerDirection);
}
float Camera::getPitch() const { return pitch; }
float Camera::getYaw() const { return yaw; }
glm::mat4 Camera::getViewMatrix() const
{
    glm::vec3 target = m_position + m_direction;
    return glm::lookAt(m_position, target, m_up);
}
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
glm::vec3 Camera::getDirection() const { return m_direction; }
glm::vec3 Camera::getRelativeOffset() const { return m_relativeOffset; }
glm::vec3 Camera::getUp() const { return m_up; }
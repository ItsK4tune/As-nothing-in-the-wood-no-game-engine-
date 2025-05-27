#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    enum class ProjectionType
    {
        Perspective,
        Orthographic
    };

    Camera(const glm::vec3 &position = glm::vec3(0.0f),
           const glm::vec3 &direction = glm::vec3(0.0f, 0.0f, 1.0f),
           const glm::vec3 &relativeOffset = glm::vec3(0.0f, 0.0f, 0.0f),
           const glm::vec3 &up = glm::vec3(0.0f, 1.0f, 0.0f),
           float fovDegrees = 90.0f,
           float aspectRatio = 16.0f / 9.0f,
           float nearPlane = 0.1f,
           float farPlane = 1000.0f);

    void setPosition(const glm::vec3 &position);
    void setDirection(const glm::vec3 &direction);
    void setRelativeOffset(const glm::vec3 &direction);
    void setUp(const glm::vec3 &up);
    void setPitchYaw(float pitch, float yaw);
    void setAspectRatio(float aspect);
    void setProjectionType(ProjectionType type);
    void setOrthoBounds(float left, float right, float bottom, float top);

    void updateFromPlayer(const glm::vec3 &playerPosition, const glm::vec3 &playerTarget);

    float getPitch() const;
    float getYaw() const;
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    glm::vec3 getPosition() const;
    glm::vec3 getDirection() const;
    glm::vec3 getRelativeOffset() const;
    glm::vec3 getUp() const;

private:
    glm::vec3 m_position;
    glm::vec3 m_relativeOffset;
    glm::vec3 m_direction;
    glm::vec3 m_up;
    float pitch = 0.0f;
    float yaw = -90.0f;

    float m_fovDegrees;
    float m_aspectRatio;
    float m_nearPlane;
    float m_farPlane;

    float m_left = -1.0f;
    float m_right = 1.0f;
    float m_bottom = -1.0f;
    float m_top = 1.0f;

    ProjectionType m_projectionType = ProjectionType::Perspective;
};
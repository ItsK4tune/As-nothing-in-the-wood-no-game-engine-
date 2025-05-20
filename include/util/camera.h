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

    Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up,
           float fovDegrees, float aspectRatio, float nearPlane, float farPlane);

    void setPosition(const glm::vec3 &position);
    void setTarget(const glm::vec3 &target);
    void setAspectRatio(float aspect);
    void setProjectionType(ProjectionType type);
    void setOrthoBounds(float left, float right, float bottom, float top);
    // void updateFromWindowSize(int width, int height);

    void move(const glm::vec3 &direction, float amount);

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    glm::vec3 getPosition() const;
    glm::vec3 getTarget() const;
    glm::vec3 getUp() const;

private:
    glm::vec3 m_position;
    glm::vec3 m_target;
    glm::vec3 m_up;

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
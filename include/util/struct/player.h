#pragma once

#include <glm/glm.hpp>
#include "entity.h"

class Player : public Entity
{
public:
    Player(const glm::vec3 &startPosition);

    void jump();
    void updateCamera(const glm::vec3 &playerPosition, const glm::vec3 &playerTarget);
    void pushBack(glm::vec3 direction, float strength);
    void update(float deltaTime, const std::vector<Vertex> &vertices, const glm::mat4 &modelMatrix);
    void setVelocity(const glm::vec3 &velocity);

    Camera &getCamera();
    glm::vec3 getVelocity() const;
    bool isGrounded() const;

private:
    Camera m_camera;
    glm::vec3 m_velocity = glm::vec3(0.0f);
    bool m_isGrounded = false;
    glm::vec3 m_lastNotCollisionPosition = getPosition();
    float m_forceXZ = 2.0f;
    float m_forceY = 2.0f;

    void applyGravity(float deltaTime);
    bool isGrounded(const glm::vec3 &direction) const;
};
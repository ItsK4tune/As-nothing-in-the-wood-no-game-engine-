#pragma once

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/intersect.hpp>
#include <glm/detail/type_vec3.hpp>

#include <random>
#include "entity.h"
#include "soundPoint.h"

class NormalEnemy : public Entity
{
public:
    NormalEnemy(const glm::vec3 &startPosition);
    void pushBack(glm::vec3 direction, float strength);
    void update(float deltaTime, const std::vector<Vertex> &vertices, const glm::mat4 &modelMatrix, std::vector<SoundPoint> &soundpoints, const glm::vec3 &playerPosition);
    void setVelocity(const glm::vec3 &velocity);
    void setDirection(const glm::vec3 &direction);

    glm::vec3 getVelocity() const;
    glm::vec3 getDirection() const;

private:
    glm::vec3 m_velocity = glm::vec3(0.0f);
    glm::vec3 m_direction = glm::vec3(0.0f, 0.0f, 0.0f);
    bool m_isGrounded = false;
    glm::vec3 m_lastNotCollisionPosition = glm::vec3(0.0f);
    float m_forceXZ = 0.5f;
    float m_forceY = 2.0f;
    float m_randomTime = 0.0f;
    float m_soundEmitTime = 0.0f;

    void randomizeVelocity();
    void applyGravity(float deltaTime);
    bool isGrounded(const glm::vec3 &direction) const;
    bool hasLineOfSight(const glm::vec3 &start, const glm::vec3 &end, const std::vector<Vertex> &vertices, const glm::mat4 &modelMatrix);
};
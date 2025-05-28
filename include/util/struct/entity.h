#pragma once

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/intersect.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "object.h"

class Entity : public Object
{
public:
    Entity(
        const Mesh &mesh = Mesh(),
        const Material &material = Material(),
        const glm::vec3 &position = glm::vec3(0.0f),
        const Shader &shader = Shader(),
        const glm::mat4 rotation = glm::mat4(1.0f),
        const glm::mat4 scale = glm::mat4(1.0f));

    void setPosition(const glm::vec3 &position) override;
    void setRotation(const glm::mat4 &rotation);
    void setScale(const glm::mat4 &scale);
    void updateAABB();
    bool checkCollisionAABB(const Entity &other) const;
    bool checkCollisionWithTriangles(const std::vector<Vertex> &vertices, const glm::mat4 &modelMatrix, glm::vec3 &outNormal);

    glm::vec3 getAABBMin() const;
    glm::vec3 getAABBMax() const;
    glm::mat4 getRotation() const;
    glm::mat4 getScale() const;

private:
    glm::vec3 m_aabbMin;
    glm::vec3 m_aabbMax;

    glm::mat4 m_rotation = glm::mat4(1.0f);
    glm::mat4 m_scale = glm::mat4(1.0f);

    bool pointInTriangle(const glm::vec3 &P, const glm::vec3 &A, const glm::vec3 &B, const glm::vec3 &C) const;
    bool hasLineOfSight(const glm::vec3 &start, const glm::vec3 &end, const glm::mat4 &modelMatrix, const glm::vec3 &A, const glm::vec3 &B, const glm::vec3 &C);
};
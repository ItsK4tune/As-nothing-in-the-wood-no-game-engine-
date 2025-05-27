#pragma once

#include <glm/glm.hpp>

#include "basic.h"
#include "../camera.h"

class Object
{
public:
    Object(
        const Mesh &mesh = Mesh(),
        const Material &material = Material(),
        const glm::vec3 &position = glm::vec3(0.0f),
        const Shader &shader = Shader());

    virtual void setPosition(const glm::vec3 &position);
    void setMesh(const Mesh &mesh);
    void setMaterial(const Material &material);
    void setAmbient(const glm::vec3 &ambient);
    void setDiffuse(const glm::vec3 &diffuse);
    void setSpecular(const glm::vec3 &specular);
    void setShininess(float shininess);
    void setShader(const Shader &shader);

    glm::vec3 getPosition() const;
    Mesh &getMesh();
    glm::vec3 getAmbient() const;
    glm::vec3 getDiffuse() const;
    glm::vec3 getSpecular() const;
    float getShininess() const;
    Shader &getShader();

    void use() const;
    void draw() const;

private:
    glm::vec3 m_position;
    Mesh m_mesh;
    Material m_material;
    Shader m_shader;
};

enum class LightType
{
    Point,
    Directional
};

// class NormalEnemy : public Entity
// {
//     NormalEnemy(const glm::vec3 &startPosition);

// public:
//     void pushBack(glm::vec3 direction, float strength);
//     void update(float deltaTime, const std::vector<Vertex> &vertices, const glm::mat4 &modelMatrix);
//     void setVelocity(const glm::vec3 &velocity);

//     glm::vec3 getVelocity() const;

// private:
//     glm::vec3 m_velocity = glm::vec3(0.0f);
//     bool m_isGrounded = false;
//     glm::vec3 m_lastNotCollisionPosition = getPosition();
//     float m_forceXZ = 2.0f;
//     float m_forceY = 2.0f;

//     void applyGravity(float deltaTime);
//     bool isGrounded(const glm::vec3 &direction) const;
// };
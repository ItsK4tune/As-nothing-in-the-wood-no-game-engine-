#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "util/struct/struct.h"
#include "util/camera.h"

class Object
{
public:
    Object(
        const Mesh &mesh = Mesh(),
        const Material &material = Material(),
        const glm::vec3 &position = glm::vec3(0.0f),
        const Shader &shader = Shader());

    void setPosition(const glm::vec3 &position);
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

class Light : public Object
{
public:
    Light(
        glm::vec3 color,
        LightType type = LightType::Point);

    void setColor(const glm::vec3 &color);
    void setType(LightType type);

    glm::vec3 getColor() const;
    LightType getType() const;

private:
    glm::vec3 m_color;
    LightType m_type;
};

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

    void setRotation(const glm::mat4 &rotation);
    void setScale(const glm::mat4 &scale);
    void updateAABB();
    bool checkCollisionAABB(const Entity &other) const;
    bool checkCollisionWithTriangles(const std::vector<Vertex> &vertices) const;

    glm::vec3 getAABBMin() const;
    glm::vec3 getAABBMax() const;
    glm::mat4 getRotation() const;
    glm::mat4 getScale() const;

private:
    glm::vec3 m_aabbMin;
    glm::vec3 m_aabbMax;

    glm::mat4 m_rotation;
    glm::mat4 m_scale;
};

class Player : public Entity
{
public:
    Player(const Camera &camera = Camera());

    void setCamera(const Camera &camera);
    Camera &getCamera();

private:
    Camera m_camera;
};
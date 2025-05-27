#pragma once

#include "basic.h"

class Line
{
public:
    Line(const Mesh &mesh = Mesh(),
         const glm::vec3 &position = glm::vec3(0.0f),
         const Shader &shader = Shader());
    // ~Line();

    void setPosition(const glm::vec3 &position);
    void setMesh(const Mesh &mesh);
    void setShader(const Shader &shader);

    glm::vec3 getPosition() const;
    Mesh &getMesh();
    Shader &getShader();

    void use() const;
    void drawLine(float thickness) const;
    void drawLineStrip(float thickness) const;
    void drawLineLoop(float thickness) const;

private:
    glm::vec3 m_position;
    Mesh m_mesh;
    Shader m_shader;
};
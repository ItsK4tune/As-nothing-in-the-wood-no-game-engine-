#include "util/struct/line.h"

Line::Line(
    const Mesh &mesh,
    const glm::vec3 &position,
    const Shader &shader)
    : m_mesh(mesh),
      m_position(position),
      m_shader(shader) {}

void Line::setPosition(const glm::vec3 &position) { m_position = position; }
void Line::setMesh(const Mesh &mesh) { m_mesh = mesh; }
void Line::setShader(const Shader &shader) { m_shader = shader; }

glm::vec3 Line::getPosition() const { return m_position; }
Mesh &Line::getMesh() { return m_mesh; }
Shader &Line::getShader() { return m_shader; }

void Line::use() const { m_shader.use(); }
void Line::drawLine(float thickness) const { m_mesh.drawLine(thickness); }
void Line::drawLineStrip(float thickness) const { m_mesh.drawLineStrip(thickness); }
void Line::drawLineLoop(float thickness) const { m_mesh.drawLineLoop(thickness); }
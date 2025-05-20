#include "util/object.h"
#include "util/struct/struct.h"

Object::Object(
    const Mesh &mesh,
    const Material &material,
    const glm::vec3 &position,
    const Shader &shader)
    : m_mesh(mesh),
      m_material(material),
      m_position(position),
      m_shader(shader) {}

void Object::setPosition(const glm::vec3 &position) { m_position = position; }
void Object::setMesh(const Mesh &mesh) { m_mesh = mesh; }
void Object::setMaterial(const Material &material) { m_material = material; }
void Object::setAmbient(const glm::vec3 &ambient) { m_material.ambient = ambient; }
void Object::setDiffuse(const glm::vec3 &diffuse) { m_material.diffuse = diffuse; }
void Object::setSpecular(const glm::vec3 &specular) { m_material.specular = specular; }
void Object::setShininess(float shininess) { m_material.shininess = shininess; }
void Object::setShader(const Shader &shader) { m_shader = shader; }

glm::vec3 Object::getPosition() const { return m_position; }
Mesh &Object::getMesh() { return m_mesh; }
glm::vec3 Object::getAmbient() const { return m_material.ambient; }
glm::vec3 Object::getDiffuse() const { return m_material.diffuse; }
glm::vec3 Object::getSpecular() const { return m_material.specular; }
float Object::getShininess() const { return m_material.shininess; }
Shader &Object::getShader() { return m_shader; }

void Object::use() const { m_shader.use(); }
void Object::draw() const { m_mesh.drawObject(); }

Light::Light(glm::vec3 color, LightType type)
    : Object(),
      m_color(color),
      m_type(type)
{
}

void Light::setColor(const glm::vec3 &color) { m_color = color; }
void Light::setType(LightType type) { m_type = type; }

glm::vec3 Light::getColor() const { return m_color; }
LightType Light::getType() const { return m_type; }
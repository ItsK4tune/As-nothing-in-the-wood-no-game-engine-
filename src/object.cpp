#include "util/object.h"
#include "util/struct/struct.h"
#include <iostream>

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

Entity::Entity(
    const Mesh &mesh,
    const Material &material,
    const glm::vec3 &position,
    const Shader &shader,
    const glm::mat4 rotation,
    const glm::mat4 scale)
    : Object(mesh, material, position, shader),
      m_rotation(rotation),
      m_scale(scale) { updateAABB(); }

void Entity::setRotation(const glm::mat4 &rotation)
{
  m_rotation = rotation;
}

void Entity::setScale(const glm::mat4 &scale)
{
  m_scale = scale;
}

glm::vec3 Entity::getAABBMin() const
{
  return m_aabbMin;
}

glm::vec3 Entity::getAABBMax() const
{
  return m_aabbMax;
}

glm::mat4 Entity::getRotation() const
{
  return m_rotation;
}

glm::mat4 Entity::getScale() const
{
  return m_scale;
}

void Entity::updateAABB()
{
  const auto &vertices = getMesh().vertices;

  glm::mat4 model = glm::translate(glm::mat4(1.0f), getPosition()) * m_rotation * m_scale;

  if (vertices.empty())
  {
    glm::vec3 origin = glm::vec3(model * glm::vec4(0, 0, 0, 1.0f));
    m_aabbMin = origin;
    m_aabbMax = origin;
    return;
  }

  glm::vec3 minP(FLT_MAX);
  glm::vec3 maxP(-FLT_MAX);

  for (const auto &v : vertices)
  {
    glm::vec3 worldPos = glm::vec3(model * glm::vec4(v.pos, 1.0f));
    minP = glm::min(minP, worldPos);
    maxP = glm::max(maxP, worldPos);
  }

  m_aabbMin = minP;
  m_aabbMax = maxP;
}

bool Entity::checkCollisionAABB(const Entity &other) const
{
  return !(m_aabbMax.x < other.m_aabbMin.x || m_aabbMin.x > other.m_aabbMax.x ||
           m_aabbMax.y < other.m_aabbMin.y || m_aabbMin.y > other.m_aabbMax.y ||
           m_aabbMax.z < other.m_aabbMin.z || m_aabbMin.z > other.m_aabbMax.z);
}

bool pointInTriangle(const glm::vec3 &P, const glm::vec3 &A, const glm::vec3 &B, const glm::vec3 &C)
{
  glm::vec3 v0 = C - A;
  glm::vec3 v1 = B - A;
  glm::vec3 v2 = P - A;

  float d00 = glm::dot(v0, v0);
  float d01 = glm::dot(v0, v1);
  float d02 = glm::dot(v0, v2);
  float d11 = glm::dot(v1, v1);
  float d12 = glm::dot(v1, v2);

  float denom = d00 * d11 - d01 * d01;
  if (denom == 0.0f)
    return false;
  float invDenom = 1.0f / denom;

  float u = (d11 * d02 - d01 * d12) * invDenom;
  float v = (d00 * d12 - d01 * d02) * invDenom;

  return (u >= 0) && (v >= 0) && (u + v <= 1);
}

bool Entity::checkCollisionWithTriangles(const std::vector<Vertex> &vertices, const glm::mat4 &modelMatrix, glm::vec3 &outPushDir) const
{
  glm::vec3 center = (m_aabbMin + m_aabbMax) * 0.5f;

  if (vertices.size() % 3 != 0)
  {
    exit(-1);
  }

  for (size_t i = 0; i < vertices.size(); i += 3)
  {
    glm::vec3 v0 = glm::vec3(modelMatrix * glm::vec4(vertices[i].pos, 1.0f));
    glm::vec3 v1 = glm::vec3(modelMatrix * glm::vec4(vertices[i + 1].pos, 1.0f));
    glm::vec3 v2 = glm::vec3(modelMatrix * glm::vec4(vertices[i + 2].pos, 1.0f));

    glm::vec3 triMin = glm::min(glm::min(v0, v1), v2);
    glm::vec3 triMax = glm::max(glm::max(v0, v1), v2);

    float delta = 0.2f;

    if (!(m_aabbMax.x + delta < triMin.x || m_aabbMin.x - delta > triMax.x ||
          m_aabbMax.y + delta < triMin.y || m_aabbMin.y - delta > triMax.y ||
          m_aabbMax.z + delta < triMin.z || m_aabbMin.z - delta > triMax.z))
    {
      if (pointInTriangle(center, v0, v1, v2))
      {
        glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
        glm::vec3 toCenter = glm::normalize(center - v0);
        float cosTheta = glm::dot(normal, toCenter);
        outPushDir = (cosTheta > 0.0f) ? normal : -normal;
        return true;
      }
    }
  }

  return false;
}

Player::Player(const glm::vec3 &startPosition)
    : m_camera(startPosition)
{
  glm::vec3 pos = getPosition();
  glm::vec3 target = m_camera.getTarget();
  updateCamera(pos, target);
}

void Player::setPosition(const glm::vec3 &position)
{
  Object::setPosition(position);
  glm::vec3 newTarget = m_camera.getTarget() + (position - getPosition());
  m_camera.setPosition(position);
  m_camera.setTarget(newTarget);

  updateAABB();
}

void Player::moveForward(float amount)
{
  glm::vec3 pos = getPosition();
  glm::vec3 forward = m_camera.getTarget() - m_camera.getPosition();
  pos += amount * glm::normalize(glm::vec3(forward.x, 0.0f, forward.z));
  setPosition(pos);
  glm::vec3 newTarget = pos + glm::normalize(forward);
  updateCamera(pos, newTarget);
  updateAABB();
}

void Player::moveRight(float amount)
{
  glm::vec3 forward = m_camera.getTarget() - m_camera.getPosition();
  glm::vec3 forwardDir = glm::normalize(glm::vec3(forward.x, 0.0f, forward.z));
  glm::vec3 up = m_camera.getUp();
  glm::vec3 right = glm::normalize(glm::cross(forwardDir, up));
  glm::vec3 pos = getPosition();
  pos += amount * right;
  setPosition(pos);
  glm::vec3 newTarget = pos + glm::normalize(forward);
  updateCamera(pos, newTarget);
  updateAABB();
}

void Player::moveUp(float amount)
{
  glm::vec3 pos = getPosition();
  pos.y += amount;
  setPosition(pos);
  glm::vec3 newTarget = m_camera.getTarget() + glm::vec3(0.0f, amount, 0.0f);
  updateCamera(pos, newTarget);
  updateAABB();
}

void Player::pushBack(glm::vec3 direction, float strength)
{
  glm::vec3 pos = getPosition();
  pos += direction * strength;
  setPosition(pos);
  glm::vec3 newTarget = m_camera.getTarget() + direction * strength;
  updateCamera(pos, newTarget);
  updateAABB();
}

void Player::updateCamera(const glm::vec3 &playerPosition, const glm::vec3 &playerTarget)
{
  m_camera.updateFromPlayer(playerPosition, playerTarget);
}

Camera &Player::getCamera()
{
  return m_camera;
}
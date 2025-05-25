#include "util/object.h"
#include "util/struct/struct.h"
#include <iostream>

#define GRAVITY 3.0f

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

void Entity::setPosition(const glm::vec3 &position)
{
  glm::vec3 oldPosition = getPosition();
  Object::setPosition(position);
  updateAABB();
}

void Entity::setRotation(const glm::mat4 &rotation)
{
  m_rotation = rotation;
}

void Entity::setScale(const glm::mat4 &scale)
{
  m_scale = scale;
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

bool Entity::pointInTriangle(const glm::vec3 &P, const glm::vec3 &A, const glm::vec3 &B, const glm::vec3 &C) const
{
  const float epsilon = 1e-5f;
  glm::vec3 v0 = C - A;
  glm::vec3 v1 = B - A;
  glm::vec3 v2 = P - A;

  float d00 = glm::dot(v0, v0);
  float d01 = glm::dot(v0, v1);
  float d02 = glm::dot(v0, v2);
  float d11 = glm::dot(v1, v1);
  float d12 = glm::dot(v1, v2);

  float denom = d00 * d11 - d01 * d01;
  if (fabs(denom) < epsilon)
    return false;

  float invDenom = 1.0f / denom;

  float u = (d11 * d02 - d01 * d12) * invDenom;
  float v = (d00 * d12 - d01 * d02) * invDenom;

  return (u >= -epsilon) && (v >= -epsilon) && (u + v <= 1.0f + epsilon);
}

bool Entity::checkCollisionWithTriangles(const std::vector<Vertex> &vertices, const glm::mat4 &modelMatrix, glm::vec3 &outNormal)
{
  if (vertices.size() % 3 != 0)
  {
    exit(-1);
  }

  glm::vec3 center = (m_aabbMin + m_aabbMax) * 0.5f;

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
      auto &entityVertices = getMesh().vertices;

      if (!entityVertices.size())
      {
        if (pointInTriangle(center, v0, v1, v2))
        {
          glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
          glm::vec3 toCenter = glm::normalize(center - v0);
          float cosTheta = glm::dot(normal, toCenter);
          outNormal = (cosTheta > 0.0f) ? normal : -normal;
          return true;
        }
      }
      else
      {
        for (auto &v : entityVertices)
        {
          if (pointInTriangle(v.pos, v0, v1, v2))
          {
            // std::cout << "Collision detected with triangle: "
            //           << "v0: " << glm::to_string(v0) << ", "
            //           << "v1: " << glm::to_string(v1) << ", "
            //           << "v2: " << glm::to_string(v2) << "vertex: " << glm::to_string(v.pos) << std::endl;
            glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
            glm::vec3 toCenter = glm::normalize(center - v0);
            float cosTheta = glm::dot(normal, toCenter);
            outNormal = (cosTheta > 0.0f) ? normal : -normal;
            return true;
          }
        }
      }
    }
  }

  return false;
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

Player::Player(const glm::vec3 &startPosition)
    : m_camera(startPosition)
{
  glm::vec3 pos = getPosition();
  glm::vec3 direction = m_camera.getDirection();
  updateCamera(pos, direction);
}

void Player::applyGravity(float deltaTime)
{
  m_velocity.y -= GRAVITY * deltaTime;
}

bool Player::isGrounded(const glm::vec3 &dir) const
{
  const float horizontalTolerance = 0.4f;
  return dir.y > 0.4f &&
         std::abs(dir.x) < horizontalTolerance &&
         std::abs(dir.z) < horizontalTolerance;
}

void Player::jump(const glm::vec3 &force)
{
  if (m_isGrounded)
  {
    m_velocity = force;
    m_isGrounded = false;
  }
}

void Player::updateCamera(const glm::vec3 &playerPosition, const glm::vec3 &direction)
{
  m_camera.updateFromPlayer(playerPosition, direction);
}

void Player::pushBack(glm::vec3 direction, float strength)
{
  glm::vec3 pos = getPosition();
  pos += direction * strength;
  setPosition(pos);
  updateCamera(pos, m_camera.getDirection());
}

void Player::update(float deltaTime, const std::vector<Vertex> &vertices, const glm::mat4 &modelMatrix)
{
  const float stepSize = 0.005f / (deltaTime * 60.8f);

  while (deltaTime > 0.0f)
  {
    float currentStep = std::min(stepSize, deltaTime);

    applyGravity(currentStep);

    glm::vec3 currentPosition = getPosition();

    glm::vec3 verticalVelocity = glm::vec3(0.0f, m_velocity.y, 0.0f);
    glm::vec3 verticalMove = verticalVelocity * currentStep * m_force;
    glm::vec3 verticalPosition = currentPosition + verticalMove;

    setPosition(verticalPosition);

    glm::vec3 collisionNormal;
    bool collidedY = checkCollisionWithTriangles(vertices, modelMatrix, collisionNormal);

    // std::cout << "Collision Normal: " << glm::to_string(collisionNormal) << std::endl;

    if (collidedY)
    {
      if (isGrounded(collisionNormal) && m_velocity.y <= 0.0f)
      {
        // std::cout << "Player is grounded." << std::endl;
        m_isGrounded = true;
        m_velocity.y = 0.0f;
        setPosition(m_lastNotCollisionPosition);
      }
      else
      {
        setPosition(m_lastNotCollisionPosition);
        m_velocity.y = 0.0f;
        m_isGrounded = false;
      }
    }
    else
    {
      m_isGrounded = false;
      m_lastNotCollisionPosition = verticalPosition;
    }

    // === PHẦN 2: Di chuyển theo XZ (trục ngang) ===
    glm::vec3 horizontalVelocity = glm::vec3(m_velocity.x, 0.0f, m_velocity.z);
    glm::vec3 horizontalMove = horizontalVelocity * currentStep * m_force;
    glm::vec3 horizontalPosition = getPosition() + horizontalMove;

    setPosition(horizontalPosition);
    // updateAABB();

    bool collidedXZ = checkCollisionWithTriangles(vertices, modelMatrix, collisionNormal);

    if (collidedXZ)
    {
      setPosition(m_lastNotCollisionPosition);
    }
    else
    {
      m_lastNotCollisionPosition = getPosition();
    }

    deltaTime -= currentStep;
  }

  // std::cout << "isGrounded: " << m_isGrounded << std::endl;
  updateCamera(getPosition(), m_camera.getDirection());
  // updateAABB();
}

void Player::setVelocity(const glm::vec3 &velocity)
{
  m_velocity.x = velocity.x;
  m_velocity.z = velocity.z;
}

Camera &Player::getCamera()
{
  return m_camera;
}

glm::vec3 Player::getVelocity() const
{
  return m_velocity;
}

bool Player::isGrounded() const
{
  return m_isGrounded;
}
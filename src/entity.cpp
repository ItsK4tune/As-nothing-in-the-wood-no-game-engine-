#include "util/struct/entity.h"

#define BROAD_PHASE_DELTA 0.01f
#define NARROW_PHASE_DELTA 0

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
    // glm::vec3 oldPosition = getPosition();
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
    glm::vec3 v0 = C - A;
    glm::vec3 v1 = B - A;
    glm::vec3 v2 = P - A;

    float d00 = glm::dot(v0, v0);
    float d01 = glm::dot(v0, v1);
    float d02 = glm::dot(v0, v2);
    float d11 = glm::dot(v1, v1);
    float d12 = glm::dot(v1, v2);

    float denom = d00 * d11 - d01 * d01;
    if (fabs(denom) < NARROW_PHASE_DELTA)
        return false;

    float invDenom = 1.0f / denom;

    float u = (d11 * d02 - d01 * d12) * invDenom;
    float v = (d00 * d12 - d01 * d02) * invDenom;

    return (u + NARROW_PHASE_DELTA >= 0) && (v + NARROW_PHASE_DELTA >= 0) && (u + v <= 1.0f + NARROW_PHASE_DELTA);
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

        if (!(m_aabbMax.x + BROAD_PHASE_DELTA < triMin.x || m_aabbMin.x - BROAD_PHASE_DELTA > triMax.x ||
              m_aabbMax.y + BROAD_PHASE_DELTA < triMin.y || m_aabbMin.y - BROAD_PHASE_DELTA > triMax.y ||
              m_aabbMax.z + BROAD_PHASE_DELTA < triMin.z || m_aabbMin.z - BROAD_PHASE_DELTA > triMax.z))
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
                    if (!hasLineOfSight(getPosition(), v.pos + getPosition(), modelMatrix, v0, v1, v2))
                    {
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

bool Entity::hasLineOfSight(const glm::vec3 &start, const glm::vec3 &end, const glm::mat4 &modelMatrix, const glm::vec3 &A, const glm::vec3 &B, const glm::vec3 &C)
{
    glm::vec3 dir = end - start;
    float maxDistance = glm::length(dir);
    if (maxDistance < 1e-4f)
        return true;

    dir = glm::normalize(dir);

    glm::vec2 uv;
    float distance;

    if (glm::intersectRayTriangle(start, dir, A, B, C, uv, distance))
    {
        if (distance > 0.0f && distance < maxDistance)
        {
            return false;
        }
    }

    return true;
}
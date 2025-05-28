#include "util/struct/normalEnemy.h"

#define STEP_FACTOR 0.4f
#define HORIZONTAL_TOLERANCE 0.4f
#define GRAVITY 3.0f
#define TIME_DELAY_TO_RANDOMIZE 3.0f
#define TIME_DELAY_TO_EMIT_SOUND 2.0f
#define CHASE_DISTANCE 3.0f

NormalEnemy::NormalEnemy(const glm::vec3 &startPosition)
{
    setPosition(startPosition);
    m_lastNotCollisionPosition = startPosition;
}

void NormalEnemy::pushBack(glm::vec3 direction, float strength)
{
    glm::vec3 pos = getPosition();
    pos += direction * strength;
    setPosition(pos);
}

void NormalEnemy::update(float deltaTime, const std::vector<Vertex> &vertices, const glm::mat4 &modelMatrix, std::vector<SoundPoint> &soundpoints, const glm::vec3 &playerPosition)
{
    m_randomTime += deltaTime;
    m_soundEmitTime += deltaTime;

    glm::vec3 currentPosition = getPosition();
    const float distanceToPlayer = glm::distance(getPosition(), playerPosition);

    if (distanceToPlayer <= CHASE_DISTANCE && hasLineOfSight(currentPosition, playerPosition, vertices, modelMatrix))
    {
        glm::vec3 diff = playerPosition - currentPosition;
        if (glm::length(diff) > 1e-4f)
        {
            glm::vec3 direction = glm::normalize(diff);
            m_velocity.x = direction.x;
            m_velocity.z = direction.z;
        }
    }
    else if (m_randomTime > TIME_DELAY_TO_RANDOMIZE)
    {
        randomizeVelocity();
        m_randomTime = 0.0f;
    }

    if (m_soundEmitTime > TIME_DELAY_TO_EMIT_SOUND)
    {
        SoundPoint soundPoint;
        soundPoint.pos = getPosition();
        soundPoint.color = glm::vec3(1.0f, 0.0f, 0.0f);
        soundPoint.maxValue = 2.0f;
        soundPoint.value = 0.1f;
        soundPoint.isGrowing = true;
        soundPoint.elapsedTime = 0.0f;

        soundpoints.push_back(soundPoint);
        m_soundEmitTime = 0.0f;
    }

    const float stepSize = STEP_FACTOR / deltaTime;

    while (deltaTime > 0.0f)
    {
        float currentStep = std::min(stepSize, deltaTime);

        applyGravity(currentStep);

        glm::vec3 currentPosition = getPosition();

        glm::vec3 verticalVelocity = glm::vec3(0.0f, m_velocity.y, 0.0f);
        glm::vec3 verticalMove = verticalVelocity * currentStep * m_forceY;
        glm::vec3 verticalPosition = currentPosition + verticalMove;

        setPosition(verticalPosition);
        glm::vec3 collisionNormal;
        bool collidedY = checkCollisionWithTriangles(vertices, modelMatrix, collisionNormal);

        if (collidedY)
        {
            if (isGrounded(collisionNormal) && m_velocity.y <= 0.0f)
            {
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
            m_lastNotCollisionPosition = getPosition();
        }

        glm::vec3 xVelocity = glm::vec3(m_velocity.x, 0.0f, 0.0f);
        glm::vec3 xMove = xVelocity * currentStep * m_forceXZ;
        glm::vec3 xPosition = getPosition() + xMove;

        setPosition(xPosition);
        bool collidedX = checkCollisionWithTriangles(vertices, modelMatrix, collisionNormal);

        if (collidedX)
        {
            setPosition(m_lastNotCollisionPosition);
        }
        else
        {
            m_lastNotCollisionPosition = getPosition();
        }

        glm::vec3 zVelocity = glm::vec3(0.0f, 0.0f, m_velocity.z);
        glm::vec3 zMove = zVelocity * currentStep * m_forceXZ;
        glm::vec3 zPosition = getPosition() + zMove;

        setPosition(zPosition);
        bool collidedZ = checkCollisionWithTriangles(vertices, modelMatrix, collisionNormal);

        if (collidedZ)
        {
            setPosition(m_lastNotCollisionPosition);
        }
        else
        {
            m_lastNotCollisionPosition = getPosition();
        }

        deltaTime -= currentStep;
    }
}

void NormalEnemy::setVelocity(const glm::vec3 &velocity)
{
    m_velocity = velocity;
}

void NormalEnemy::setDirection(const glm::vec3 &direction)
{
    m_direction = direction;
}

glm::vec3 NormalEnemy::getVelocity() const
{
    return m_velocity;
}

glm::vec3 NormalEnemy::getDirection() const
{
    return m_direction;
}

void NormalEnemy::applyGravity(float deltaTime)
{
    if (!m_isGrounded)
    {
        m_velocity.y -= GRAVITY * deltaTime;
    }
}

bool NormalEnemy::isGrounded(const glm::vec3 &direction) const
{
    return glm::abs(direction.y) < HORIZONTAL_TOLERANCE;
}

void NormalEnemy::randomizeVelocity()
{
    static std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> dist(0.0f, glm::two_pi<float>());

    float angle = dist(rng);
    m_velocity = glm::vec3(cos(angle), 0.0f, sin(angle));
}

bool NormalEnemy::hasLineOfSight(const glm::vec3 &start, const glm::vec3 &end, const std::vector<Vertex> &vertices, const glm::mat4 &modelMatrix)
{
    glm::vec3 dir = end - start;
    float maxDistance = glm::length(dir);
    if (maxDistance < 1e-4f)
        return true;

    dir = glm::normalize(dir);

    for (size_t i = 0; i + 2 < vertices.size(); i += 3)
    {
        glm::vec3 v0 = glm::vec3(modelMatrix * glm::vec4(vertices[i].pos, 1.0f));
        glm::vec3 v1 = glm::vec3(modelMatrix * glm::vec4(vertices[i + 1].pos, 1.0f));
        glm::vec3 v2 = glm::vec3(modelMatrix * glm::vec4(vertices[i + 2].pos, 1.0f));

        glm::vec2 uv;
        float distance;

        if (glm::intersectRayTriangle(start, dir, v0, v1, v2, uv, distance))
        {
            if (distance > 0.0f && distance < maxDistance)
            {
                return false;
            }
        }
    }

    return true;
}
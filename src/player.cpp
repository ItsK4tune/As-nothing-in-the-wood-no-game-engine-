#include "util/struct/player.h"

#define STEP_FACTOR 0.4f
#define HORIZONTAL_TOLERANCE 0.4f
#define GRAVITY 3.0f

Player::Player(const glm::vec3 &startPosition)
{
    glm::vec3 pos = getPosition();
    glm::vec3 direction = m_camera.getDirection();
    updateCamera(pos, direction);
    m_lastNotCollisionPosition = startPosition;
}

void Player::applyGravity(float deltaTime)
{
    m_velocity.y -= GRAVITY * deltaTime;
}

bool Player::isGrounded(const glm::vec3 &dir) const
{
    return dir.y > 0.4f &&
           std::abs(dir.x) < HORIZONTAL_TOLERANCE &&
           std::abs(dir.z) < HORIZONTAL_TOLERANCE;
}

void Player::jump()
{
    if (m_isGrounded)
    {
        m_velocity = glm::vec3(m_velocity.x, 1.0f, m_velocity.z);
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

    updateCamera(getPosition(), m_camera.getDirection());
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
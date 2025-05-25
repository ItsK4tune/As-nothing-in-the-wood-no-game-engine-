#include "util/input.h"
#include "util/affine.h"

#define SENSITIVITY 0.07f
#define WALKING_MULTIPLIER 1.0f
#define RUNNING_MULTIPLIER 2.0f
#define IDLE_MULTIPLIER 0.05f
#define THRESHOLD 0.7f
#define RUNNING_SPEED 0.6f
#define WALKING_SPEED 0.3f

Player *mainCharacter = nullptr;

glm::vec3 objectRotation = glm::vec3(0.0f);
glm::vec3 objectScale = glm::vec3(1.0f);
glm::mat4 model = glm::mat4(1.0f);

void mouseInput(GLFWwindow *window, double xpos, double ypos)
{
    static bool firstMouse = true;
    static double lastX = 0.0, lastY = 0.0;

    Camera &camera = mainCharacter->getCamera();
    float pitch = camera.getPitch();
    float yaw = camera.getYaw();

    // if (firstMouse)
    // {
    //     glm::vec3 dir = mainCharacter->getCamera().getTarget();
    //     pitch = glm::degrees(asin(dir.y));
    //     yaw = glm::degrees(atan2(dir.z, dir.x));
    //     lastX = xpos;
    //     lastY = ypos;
    //     firstMouse = false;

    //     camera.setPitchYaw(pitch, yaw);
    //     return;
    // }

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    double dx = xpos - lastX;
    double dy = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    yaw += dx * SENSITIVITY;
    pitch += dy * SENSITIVITY;

    pitch = glm::clamp(pitch, -89.0f, 89.0f);

    glm::vec3 direction;
    direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    direction.y = sin(glm::radians(pitch));
    direction.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    direction = glm::normalize(direction);

    glm::vec3 pos = mainCharacter->getPosition();
    camera.updateFromPlayer(pos, direction);
    camera.setPitchYaw(pitch, yaw);
}

// void ghostMoveInput(GLFWwindow *window, float deltaTime)
// {
//     float baseSpeed = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) ? 1.0f : 0.5f;
//     float speed = baseSpeed * deltaTime;
//     float angleStep = 1.0f * deltaTime;
//     float scaleStep = 1.0f * deltaTime;

//     if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//         mainCharacter->moveForward(speed);
//     if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//         mainCharacter->moveForward(-speed);
//     if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//         mainCharacter->moveRight(-speed);
//     if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//         mainCharacter->moveRight(speed);
//     if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
//         mainCharacter->moveUp(speed);
//     if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
//         mainCharacter->moveUp(-speed);

//     if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
//         objectRotation.x -= angleStep;
//     if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
//         objectRotation.x += angleStep;
//     if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
//         objectRotation.y -= angleStep;
//     if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
//         objectRotation.y += angleStep;

//     if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
//         objectScale += glm::vec3(scaleStep);
//     if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
//     {
//         objectScale -= glm::vec3(scaleStep);
//         objectScale = glm::max(objectScale, glm::vec3(0.1f));
//     }

//     model = createAffineTransformMatrix(objectScale, objectRotation, glm::vec3(0.0f));
// }

void normalMoveInput(GLFWwindow *window, std::vector<SoundPoint> &soundpoints, float deltaTime)
{
    static float moveTimeAccumulator = 0.0f;

    bool isRunning = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);
    bool isMoving = false;
    float speed = (isRunning ? RUNNING_SPEED : WALKING_SPEED);

    glm::vec3 forward = mainCharacter->getCamera().getDirection();
    glm::vec3 up = mainCharacter->getCamera().getUp();
    glm::vec3 right = glm::normalize(glm::cross(forward, up));
    glm::vec3 moveInput(0.0f);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        moveInput += glm::vec3(forward.x, 0.0f, forward.z);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        moveInput -= glm::vec3(forward.x, 0.0f, forward.z);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        moveInput += right;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        moveInput -= right;

    if (glm::length(moveInput) > 0.0f)
    {
        moveInput = glm::normalize(moveInput) * speed;
        mainCharacter->setVelocity(moveInput);
        isMoving = true;
    }
    else
    {
        mainCharacter->setVelocity(glm::vec3(0.0f));
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && mainCharacter->isGrounded())
    {
        mainCharacter->jump(glm::vec3(0.0f, 0.8f, 0.0f));
        SoundPoint sp;
        sp.pos = mainCharacter->getPosition();
        sp.maxValue = isRunning ? 1.0f : 0.75f;
        sp.value = 0.f;
        sp.isGrowing = true;
        soundpoints.push_back(sp);
    }

    if (isMoving)
    {
        moveTimeAccumulator += (isRunning ? RUNNING_MULTIPLIER : WALKING_MULTIPLIER) * deltaTime;
    }
    else
    {
        moveTimeAccumulator -= IDLE_MULTIPLIER * deltaTime;
        if (moveTimeAccumulator < 0.0f)
            moveTimeAccumulator = 0.0f;
    }

    while (moveTimeAccumulator >= THRESHOLD)
    {
        SoundPoint sp;
        sp.pos = mainCharacter->getPosition();
        sp.maxValue = isRunning ? 1.0f : 0.75f;
        sp.value = 0.f;
        sp.isGrowing = true;
        soundpoints.push_back(sp);

        moveTimeAccumulator -= THRESHOLD;
    }
}

void exitInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void soundWaveInput(GLFWwindow *window, std::vector<SoundPoint> &soundpoints)
{
    static bool wasPressed = false;

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        if (!wasPressed)
        {
            SoundPoint sp;
            sp.pos = mainCharacter->getPosition();
            sp.maxValue = 1.5f;
            sp.value = 0.4f;
            sp.isGrowing = true;
            soundpoints.push_back(sp);
            wasPressed = true;
        }
    }
    else
    {
        wasPressed = false;
    }
}
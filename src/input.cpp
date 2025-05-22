#include "util/input.h"
#include "util/affine.h"

Camera *globalCamera = nullptr;

float pitch = 0.0f, yaw = -90.0f;
glm::vec3 objectRotation = glm::vec3(0.0f);
glm::vec3 objectScale = glm::vec3(1.0f);
glm::mat4 model = glm::mat4(1.0f);

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
    static bool firstMouse = true;
    static double lastX = 0.0, lastY = 0.0;

    if (firstMouse)
    {
        glm::vec3 dir = glm::normalize(globalCamera->getTarget() - globalCamera->getPosition());
        pitch = glm::degrees(asin(dir.y));
        yaw = glm::degrees(atan2(dir.z, dir.x));
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    double dx = xpos - lastX;
    double dy = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    yaw += dx * sensitivity;
    pitch += dy * sensitivity;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    direction.y = sin(glm::radians(pitch));
    direction.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

    if (globalCamera)
    {
        glm::vec3 position = globalCamera->getPosition();
        globalCamera->setTarget(position + glm::normalize(direction));
    }
}

void ghostMoveInput(GLFWwindow *window, float deltaTime)
{
    if (!globalCamera)
        return;

    float baseSpeed = 1.0f;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        baseSpeed = 2.0f;

    float speed = baseSpeed * deltaTime;
    float angleStep = 1.0f * deltaTime;
    float scaleStep = 1.0f * deltaTime;

    glm::vec3 forward = glm::normalize(globalCamera->getTarget() - globalCamera->getPosition());
    glm::vec3 right = glm::normalize(glm::cross(forward, globalCamera->getUp()));
    glm::vec3 up = globalCamera->getUp();

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        globalCamera->move(forward, speed);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        globalCamera->move(-forward, speed);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        globalCamera->move(-right, speed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        globalCamera->move(right, speed);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        globalCamera->move(up, speed);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        globalCamera->move(-up, speed);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        objectRotation.x -= angleStep;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        objectRotation.x += angleStep;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        objectRotation.y -= angleStep;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        objectRotation.y += angleStep;
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
        objectScale += glm::vec3(scaleStep);
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
    {
        objectScale -= glm::vec3(scaleStep);
        objectScale = glm::max(objectScale, glm::vec3(0.1f));
    }

    model = createAffineTransformMatrix(objectScale, objectRotation, glm::vec3(0.0f));
}

void normalMoveInput(GLFWwindow *window, float deltaTime)
{
    if (!globalCamera)
        return;

    float baseSpeed = 1.0f;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        baseSpeed = 2.0f;

    float speed = baseSpeed * deltaTime;

    if (!speed)
        return;

    glm::vec3 forward = glm::normalize(glm::vec3(globalCamera->getTarget().x, globalCamera->getPosition().y, globalCamera->getTarget().z) - globalCamera->getPosition());
    glm::vec3 right = glm::normalize(glm::cross(forward, globalCamera->getUp()));
    glm::vec3 up = globalCamera->getUp();

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        globalCamera->move(forward, speed);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        globalCamera->move(-forward, speed);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        globalCamera->move(-right, speed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        globalCamera->move(right, speed);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        globalCamera->move(up, speed);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        globalCamera->move(-up, speed);
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
            sp.pos = globalCamera->getPosition();
            sp.maxValue = 2.0f;
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
#include "util/init.h"
#include "util/input.h"
#include <iostream>

GLFWwindow *createWindow(int width, int height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window;

    if (width < 0 || height < 0)
    {
        GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(primaryMonitor);

        window = glfwCreateWindow(mode->width, mode->height, "As nothing in the wood", primaryMonitor, NULL);
    }
    else
    {
        window = glfwCreateWindow(width, height, "As nothing in the wood", NULL, NULL);
    }

    if (!window)
    {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);

    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    gladLoadGL();

    return window;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void configWindow(GLFWwindow *window)
{
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, cursor_position_callback);
}
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <sstream>

#include "util/init.h"
#include "util/line.h"
#include "util/object.h"
#include "util/camera.h"
#include "util/input.h"
#include "util/loadShader.h"
#include "util/loadVertex.h"

int main()
{
    GLFWwindow *window = createWindow(-1, -1);
    configWindow(window);

    Camera camera(
        glm::vec3(0.0f, 0.0f, -6.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        90.0f,
        16.0f / 9.0f,
        0.1f,
        100.0f);
    globalCamera = &camera;

    // Viền
    Line outline;

    std::vector<Vertex> vertices;
    loadVertexFile("line.vertex", vertices);

    Mesh outlineMesh(vertices);
    outline.setMesh(outlineMesh);

    Shader outlineShader("outline/basic.vert", "outline/basic.frag");
    outline.setShader(outlineShader);

    // Main object
    Object box;

    loadVertexFile("box.vertex", vertices);

    Mesh boxMesh(vertices);
    box.setMesh(boxMesh);

    Shader boxShader("box/basic.vert", "box/basic.frag");
    box.setShader(boxShader);

    double lastTime = glfwGetTime();
    int frames = 0;

    float lastFrame = glfwGetTime();

    float soundRadius = 0.1f;

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        float delta = currentFrame - lastFrame;
        lastFrame = currentFrame;

        frames++;
        if (currentFrame - lastTime >= 1.0)
        {
            std::stringstream ss;
            ss << "As nothing in the wood - FPS: " << frames;
            glfwSetWindowTitle(window, ss.str().c_str());
            frames = 0;
            lastTime += 1.0;
        }

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = camera.getProjectionMatrix();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        bool triggerSoundRing = soundWaveInput(window, soundRadius, delta);

        box.use();

        box.getShader().setMat4("model", model);
        box.getShader().setMat4("view", view);
        box.getShader().setMat4("projection", projection);
        box.getShader().setVec3("cameraPos", camera.getPosition());
        box.getShader().setFloat("soundRadius", soundRadius);
        if (triggerSoundRing)
            box.getShader().setBool("useColor", 0);
        else
            box.getShader().setBool("useColor", 1);

        box.draw();

        outline.use();

        outline.getShader().setMat4("model", model);
        outline.getShader().setMat4("view", view);
        outline.getShader().setMat4("projection", projection);
        outline.getShader().setVec3("cameraPos", camera.getPosition());
        outline.getShader().setFloat("soundRadius", soundRadius);
        outline.getShader().setBool("useColor", 0);

        outline.drawLine(10.0f);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
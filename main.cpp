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

#include "util/struct/soundPoint.h"

int main()
{
    GLFWwindow *window = createWindow(1920, 1080);
    configWindow(window);

    Camera camera(
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        90.0f,
        16.0f / 9.0f,
        0.1f,
        100.0f);
    globalCamera = &camera;

    // Main object
    Object box;

    std::vector<Vertex> vertices;
    loadVertexFile("box.vertex", vertices);
    Mesh boxMesh(vertices);
    box.setMesh(boxMesh);

    Shader boxFace("box/basic.vert", "box/basic.frag");
    Shader boxOutline("outline/basic.vert", "outline/basic.frag", "outline/basic.geom");

    double lastTime = glfwGetTime();
    int frames = 0;

    float lastFrame = glfwGetTime();

    std::vector<SoundPoint> SoundPoints;

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
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

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        // glEnable(GL_CULL_FACE);
        // glCullFace(GL_FRONT);
        // glFrontFace(GL_CW);

        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = camera.getProjectionMatrix();

        moveInput(window, deltaTime);
        exitInput(window);
        soundWaveInput(window, SoundPoints);

        for (auto it = SoundPoints.begin(); it != SoundPoints.end();)
        {
            if (it->isGrowing)
            {
                it->value += deltaTime * 5;
                if (it->value >= it->maxValue)
                {
                    it->value = it->maxValue;
                    it->isGrowing = false;
                }
                ++it;
            }
            else
            {
                it->value -= deltaTime;
                if (it->value <= 0.0f)
                    it = SoundPoints.erase(it);
                else
                    ++it;
            }
        }

        const int MAX_SOUND_POINTS = 10;
        std::vector<glm::vec3> positions;
        std::vector<float> radii;

        for (int i = 0; i < std::min((int)SoundPoints.size(), MAX_SOUND_POINTS); ++i)
        {
            positions.push_back(SoundPoints[i].pos);
            radii.push_back(SoundPoints[i].value);
        }

        box.setShader(boxFace);
        box.use();

        box.getShader().setMat4("model", model);
        box.getShader().setMat4("view", view);
        box.getShader().setMat4("projection", projection);
        box.getShader().setInt("soundCount", positions.size());
        box.getShader().setVec3Array("soundPositions", positions);
        box.getShader().setFloatArray("soundRadii", radii);
        box.getShader().setBool("useColor", 0);

        box.draw();

        box.setShader(boxOutline);
        box.use();

        box.getShader().setMat4("model", model);
        box.getShader().setMat4("view", view);
        box.getShader().setMat4("projection", projection);
        box.getShader().setInt("soundCount", positions.size());
        box.getShader().setVec3Array("soundPositions", positions);
        box.getShader().setFloatArray("soundRadii", radii);
        box.getShader().setBool("useColor", 0);

        box.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
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
    GLFWwindow *window = createWindow(-1, -1);
    configWindow(window);

    Player player(glm::vec3(0.0f, 0.0f, 0.0f));
    player.setPosition(glm::vec3(-0.5f, 0.5f, -0.5f));
    mainCharacter = &player;

    Object box;

    std::vector<Vertex> vertices;
    loadVertexFile("house.vertex", vertices);
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

        glm::mat4 view = player.getCamera().getViewMatrix();
        glm::mat4 projection = player.getCamera().getProjectionMatrix();

        normalMoveInput(window, SoundPoints, deltaTime);
        exitInput(window);
        soundWaveInput(window, SoundPoints);

        for (auto it = SoundPoints.begin(); it != SoundPoints.end();)
        {
            if (!it->update(deltaTime))
                it = SoundPoints.erase(it);
            else
                ++it;
        }

        const int MAX_SOUND_POINTS = 100;
        std::vector<glm::vec3> positions;
        std::vector<float> radii;
        std::vector<float> maxRadii;
        std::vector<bool> isGrowings;

        for (int i = 0; i < std::min((int)SoundPoints.size(), MAX_SOUND_POINTS); ++i)
        {
            positions.push_back(SoundPoints[i].pos);
            radii.push_back(SoundPoints[i].value);
            maxRadii.push_back(SoundPoints[i].maxValue);
            isGrowings.push_back(SoundPoints[i].isGrowing);
        }

        box.setShader(boxFace);
        box.use();

        box.getShader().setMat4("model", model);
        box.getShader().setMat4("view", view);
        box.getShader().setMat4("projection", projection);
        box.getShader().setInt("soundCount", positions.size());
        box.getShader().setVec3Array("soundPositions", positions);
        box.getShader().setFloatArray("soundRadii", radii);
        box.getShader().setFloatArray("soundMaxRadii", maxRadii);
        box.getShader().setBoolArray("soundIsGrowing", isGrowings);
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
        box.getShader().setFloatArray("soundMaxRadii", maxRadii);
        box.getShader().setBool("useColor", 0);

        box.draw();

        glm::vec3 pushDir;
        if (player.checkCollisionWithTriangles(vertices, model, pushDir))
        {
            float pushStrength = 0.001f;
            player.pushBack(pushDir, pushStrength);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
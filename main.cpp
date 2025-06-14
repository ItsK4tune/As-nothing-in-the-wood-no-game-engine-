#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <sstream>

#include "util/init.h"
#include "util/struct.h"
#include "util/camera.h"
#include "util/input.h"
#include "util/loadShader.h"
#include "util/loadVertex.h"

int main()
{
    GLFWwindow *window = createWindow(1920, 16.0f / 9.0f);
    configWindow(window);

    Player player(glm::vec3(0.5f, 0.5f, 0.5f));
    player.getCamera().setRelativeOffset(glm::vec3(0.0f, 0.1f, 0.0f));
    // player.getCamera().set(Camera::ProjectionType::Perspective);
    mainCharacter = &player;

    std::vector<Vertex> playerVertices;
    loadVertexFile("player.vertex", playerVertices);
    Mesh playerMesh(playerVertices);
    player.setMesh(playerMesh);
    Shader playerOutline("outline/basic.vert", "outline/basic.frag", "outline/basic.geom");
    player.setShader(playerOutline);

    NormalEnemy enemy(glm::vec3(0.5f, 0.5f, 0.5f));
    std::vector<Vertex> enemyVertices;
    loadVertexFile("player.vertex", enemyVertices);
    Mesh enemyMesh(enemyVertices);
    enemy.setMesh(enemyMesh);
    Shader enemyOutline("outline/basic.vert", "outline/basic.frag", "outline/basic.geom");
    enemy.setShader(playerOutline);

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

    std::vector<SoundPoint> soundPoints;

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

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        mouseInput(window, xpos, ypos);
        normalMoveInput(window, soundPoints, deltaTime);
        exitInput(window);
        soundWaveInput(window, soundPoints);
        player.update(deltaTime, vertices, model);
        enemy.update(deltaTime, vertices, model, soundPoints, player.getPosition());

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glm::mat4 view = player.getCamera().getViewMatrix();
        glm::mat4 projection = player.getCamera().getProjectionMatrix();

        for (auto it = soundPoints.begin(); it != soundPoints.end();)
        {
            if (!it->update(deltaTime))
                it = soundPoints.erase(it);
            else
                ++it;
        }

        const int MAX_SOUND_POINTS = 100;
        std::vector<glm::vec3> positions;
        std::vector<float> radii;
        std::vector<float> maxRadii;
        std::vector<bool> isGrowings;
        std::vector<glm::vec3> colors;

        for (int i = 0; i < std::min((int)soundPoints.size(), MAX_SOUND_POINTS); ++i)
        {
            positions.push_back(soundPoints[i].pos);
            radii.push_back(soundPoints[i].value);
            maxRadii.push_back(soundPoints[i].maxValue);
            isGrowings.push_back(soundPoints[i].isGrowing);
            colors.push_back(soundPoints[i].color);
        }

        box.setShader(boxFace);
        box.use();

        box.getShader().setMat4("model", model);
        box.getShader().setMat4("view", view);
        box.getShader().setMat4("projection", projection);
        box.getShader().setInt("soundCount", positions.size());
        box.getShader().setVec3Array("soundPositions", positions);
        box.getShader().setVec3Array("soundColors", colors);
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
        box.getShader().setVec3Array("soundColors", colors);
        box.getShader().setFloatArray("soundRadii", radii);
        box.getShader().setFloatArray("soundMaxRadii", maxRadii);
        box.getShader().setBool("useColor", 0);

        box.draw();

        player.use();

        player.getShader().setMat4("model", glm::translate(glm::mat4(1.0f), player.getPosition()) * player.getRotation() * player.getScale());
        player.getShader().setMat4("view", view);
        player.getShader().setMat4("projection", projection);
        player.getShader().setInt("soundCount", positions.size());
        player.getShader().setVec3Array("soundPositions", positions);
        player.getShader().setVec3Array("soundColors", colors);
        player.getShader().setFloatArray("soundRadii", radii);
        player.getShader().setFloatArray("soundMaxRadii", maxRadii);
        player.getShader().setBool("useColor", 0);

        player.draw();

        enemy.use();

        enemy.getShader().setMat4("model", glm::translate(glm::mat4(1.0f), enemy.getPosition()) * enemy.getRotation() * enemy.getScale());
        enemy.getShader().setMat4("view", view);
        enemy.getShader().setMat4("projection", projection);
        enemy.getShader().setInt("soundCount", positions.size());
        enemy.getShader().setVec3Array("soundPositions", positions);
        enemy.getShader().setVec3Array("soundColors", colors);
        enemy.getShader().setFloatArray("soundRadii", radii);
        enemy.getShader().setFloatArray("soundMaxRadii", maxRadii);
        enemy.getShader().setBool("useColor", 0);

        enemy.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
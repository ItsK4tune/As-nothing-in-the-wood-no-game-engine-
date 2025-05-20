#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include "util/init.h"
#include "util/line.h"
#include "util/camera.h"

int main()
{
    GLFWwindow *window = createWindow(800, 600);

    Camera camera(
        glm::vec3(0.0f, 0.0f, -6.0f), // position
        glm::vec3(0.0f, 0.0f, 1.0f),  // target (hướng nhìn)
        glm::vec3(0.0f, 1.0f, 0.0f),  // up vector
        90.0f,                        // FOV (độ)
        16.0f / 9.0f,                 // aspect ratio
        0.1f,                         // near plane
        100.0f                        // far plane
    );

    Line simpleLine;

    std::vector<Vertex> vertices;
    vertices.push_back({glm::vec3(0.5f, 0.0f, 0.0f), glm::vec2(0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)});
    vertices.push_back({glm::vec3(-0.5f, 0.0f, 0.0f), glm::vec2(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)});
    vertices.push_back({glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec2(0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)});
    vertices.push_back({glm::vec3(0.5f, 0.5f, 0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)});

    Mesh simpleLineMesh(vertices);

    simpleLine.setMesh(simpleLineMesh);

    Shader simpleLineShader("shader/line/basic.vert", "shader/line/basic.frag");

    simpleLine.setShader(simpleLineShader);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, simpleLine.getPosition());

        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = camera.getProjectionMatrix();

        simpleLine.use();

        simpleLine.getShader().setMat4("model", model);
        simpleLine.getShader().setMat4("view", view);
        simpleLine.getShader().setMat4("projection", projection);

        simpleLine.drawLineLoop(20.0f);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
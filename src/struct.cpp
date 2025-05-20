#include <iostream>
#include <cstdlib>

#include "util/struct/struct.h"
#include "util/loadShader.h"

Mesh::Mesh(const std::vector<Vertex> &verts)
    : vertices(verts)
{
    setupMesh();
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Mesh::drawObject() const
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);
}

void Mesh::drawLine(float thickness) const
{
    if (vertices.size() % 2 != 0)
    {
        std::cerr << "[drawLine] The number of vertices (" << vertices.size() << ") is not divisible by 2. Cannot render using GL_LINES.\n";
        std::exit(EXIT_FAILURE);
    }
    glLineWidth(thickness);
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, vertices.size());
    glBindVertexArray(0);
}

void Mesh::drawLineStrip(float thickness) const
{
    glLineWidth(thickness);
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINE_STRIP, 0, vertices.size());
    glBindVertexArray(0);
}

void Mesh::drawLineLoop(float thickness) const
{
    glLineWidth(thickness);
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINE_LOOP, 0, vertices.size());
    glBindVertexArray(0);
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, pos));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uv));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, color));
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);
}

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
    ID = createShaderProgram(vertexPath, fragmentPath);
}

Shader::~Shader()
{
    if (ID != 0)
        glDeleteProgram(ID);
}

void Shader::use() const
{
    glUseProgram(ID);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &vec) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &vec[0]);
}

void Shader::setFloat(const std::string &name, float val) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), val);
}

unsigned int Shader::getID() const
{
    return ID;
}
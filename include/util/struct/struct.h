#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <cmath>
#include <string>

struct Vertex
{
    glm::vec3 pos = glm::vec3(0.0f);
    glm::vec2 uv = glm::vec2(0.0f);
    glm::vec3 normal = glm::vec3(0.0f);

    glm::vec3 color = glm::vec3(1.0f);
};

struct Material
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

struct Mesh
{
    std::vector<Vertex> vertices;
    unsigned int VAO = 0, VBO = 0;

    Mesh() {};
    Mesh(const std::vector<Vertex> &verts);
    ~Mesh();

    void drawObject() const;
    void drawLine(float thickness) const;
    void drawLineStrip(float thickness) const;
    void drawLineLoop(float thickness) const;
    void setupMesh();
};

struct Shader
{
    unsigned int ID = 0;

    Shader() {};
    Shader(const std::string vertexPath, const std::string fragmentPath);
    ~Shader();

    void use() const;

    void setMat4(const std::string &name, const glm::mat4 &mat) const;
    void setVec3(const std::string &name, const glm::vec3 &vec) const;
    void setFloat(const std::string &name, float val) const;
    void setInt(const std::string &name, int val) const;
    void setBool(const std::string &name, bool val) const;

    unsigned int getID() const;
};
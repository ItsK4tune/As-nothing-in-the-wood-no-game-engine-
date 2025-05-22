#include "util/loadShader.h"
#include <iostream>
#include <fstream>
#include <sstream>

std::string loadShaderSource(const std::string filepath)
{
    std::string fullPath = std::string("../shader/") + filepath;

    std::ifstream file(fullPath);
    if (!file.is_open())
    {
        std::cerr << "Cannot read shader file: " << filepath << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

unsigned int compileShader(unsigned int type, const char *source)
{
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation failed\n"
                  << infoLog << std::endl;
    }

    return shader;
}

unsigned int createShaderProgram(const std::string inVert, const std::string inFrag, const std::string inGeo = "")
{
    std::string vertexSource = loadShaderSource(inVert);
    std::string fragmentSource = loadShaderSource(inFrag);

    const char *vertexShaderSource = vertexSource.c_str();
    const char *fragmentShaderSource = fragmentSource.c_str();

    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    unsigned int geometryShader = 0;
    if (!inGeo.empty())
    {
        std::string geometrySource = loadShaderSource(inGeo);
        const char *geometryShaderSource = geometrySource.c_str();
        geometryShader = compileShader(GL_GEOMETRY_SHADER, geometryShaderSource);
    }

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    if (geometryShader != 0)
        glAttachShader(shaderProgram, geometryShader);

    glLinkProgram(shaderProgram);

    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader program linking failed\n"
                  << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    if (geometryShader != 0)
        glDeleteShader(geometryShader);

    return shaderProgram;
}

#pragma once

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

unsigned int createShaderProgram(const std::string inVert, const std::string inFrag, const std::string geometryPath);
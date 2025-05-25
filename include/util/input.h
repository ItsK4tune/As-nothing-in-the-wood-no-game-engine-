#pragma once

#include <GLFW/glfw3.h>
#include <vector>
#include "util/object.h"

#include "util/struct/soundPoint.h"

extern Player *mainCharacter;
extern glm::mat4 model;

void mouseInput(GLFWwindow *window, double xpos, double ypos);
void ghostMoveInput(GLFWwindow *window, float deltaTime);
void normalMoveInput(GLFWwindow *window, std::vector<SoundPoint> &soundpoints, float deltaTime);
void exitInput(GLFWwindow *window);
void soundWaveInput(GLFWwindow *window, std::vector<SoundPoint> &soundpoints);
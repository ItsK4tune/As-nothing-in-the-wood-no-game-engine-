#pragma once

#include <GLFW/glfw3.h>
#include <vector>
#include "util/camera.h"

#include "util/struct/soundPoint.h"

extern Camera *globalCamera;
extern glm::mat4 model;

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
void ghostMoveInput(GLFWwindow *window, float deltaTime);
void normalMoveInput(GLFWwindow *window, float deltaTime);
void exitInput(GLFWwindow *window);
void soundWaveInput(GLFWwindow *window, std::vector<SoundPoint> &soundpoints);
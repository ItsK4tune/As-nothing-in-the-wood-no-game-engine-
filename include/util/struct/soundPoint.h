#pragma once

#include <glm/glm.hpp>
#include <cmath>

struct SoundPoint
{
    glm::vec3 pos;
    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
    float maxValue = 5.0f;
    float value = 0.1f;
    bool isGrowing = true;
    float elapsedTime = 0.0f;

    bool update(float deltaTime);
};
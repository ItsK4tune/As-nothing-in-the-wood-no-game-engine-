#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct SoundPoint
{
    glm::vec3 pos;
    float maxValue = 5.0f;
    float value = 0.1f;
    bool isGrowing = true;
    float elapsedTime = 0.0f;

    bool update(float deltaTime);
};
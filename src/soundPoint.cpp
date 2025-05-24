#include "util/struct/soundPoint.h"
#include <cmath>

bool SoundPoint::update(float deltaTime)
{
    if (isGrowing)
    {
        elapsedTime += deltaTime;
        float k = 3.0f;
        value = maxValue * (1.0f - std::exp(-k * elapsedTime));

        if (value >= maxValue - 0.01f)
        {
            value = maxValue;
            isGrowing = false;
        }
    }
    else
    {
        float ratio = value / maxValue;
        value -= deltaTime * (1.01f - std::pow(ratio, 2.0f)) * 2;
        if (value <= 0.0f)
            return false;
    }
    return true;
}
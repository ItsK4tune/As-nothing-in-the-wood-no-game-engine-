#include "util/struct/soundPoint.h"
#include <iostream>

bool SoundPoint::update(float deltaTime)
{
    if (isGrowing)
    {
        value += deltaTime * 2;
        if (value >= maxValue)
        {
            value = maxValue;
            isGrowing = false;
        }
    }
    else
    {
        float ratio = value / maxValue;
        value -= deltaTime * (1.01f - pow(ratio, 2.0f)) * 2;
        if (value <= 0.0f)
            return false;
    }
    return true;
}
#include "util/struct/light.h"

Light::Light(glm::vec3 color, LightType type)
    : Object(),
      m_color(color),
      m_type(type)
{
}

void Light::setColor(const glm::vec3 &color) { m_color = color; }
void Light::setType(LightType type) { m_type = type; }

glm::vec3 Light::getColor() const { return m_color; }
LightType Light::getType() const { return m_type; }
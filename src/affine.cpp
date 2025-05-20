#include "util/affine.h"

glm::mat4 createRotationMatrix(glm::vec3 rotationRadians)
{
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::rotate(model, rotationRadians.x, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, rotationRadians.y, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, rotationRadians.z, glm::vec3(0.0f, 0.0f, 1.0f));

    return model;
}

glm::mat4 createTranslationMatrix(glm::vec3 translation)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, translation);
    return model;
}

glm::mat4 createScalingMatrix(glm::vec3 scale)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, scale);
    return model;
}

glm::mat4 createAffineTransformMatrix(
    glm::vec3 scale,
    glm::vec3 rotationRadians,
    glm::vec3 translation)
{
    glm::mat4 model = glm::mat4(1.0f);

    model = createScalingMatrix(scale) * model;
    model = createRotationMatrix(rotationRadians) * model;
    model = createTranslationMatrix(translation) * model;

    return model;
}
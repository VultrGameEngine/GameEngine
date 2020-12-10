#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
struct TransformComponent
{
    glm::vec3 position = glm::vec3(0, 0, 0);
    glm::quat rotation = glm::quat(0, 0, 0, 0);
    glm::vec3 scale = glm::vec3(1, 1, 1);
};
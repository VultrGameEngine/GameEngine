#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
struct TransformComponent
{
    glm::vec3 position = glm::vec3(0, 0, 0);
    glm::quat rotation = glm::quat(0, 0, 0, 1);
    glm::vec3 scale = glm::vec3(1, 1, 1);

    inline glm::vec3 Forward()
    {
        return rotation * glm::vec3(0, 0, 1);
    }

    inline glm::vec3 Right()
    {
        return rotation * glm::vec3(-1, 0, 0);
    }
    inline glm::vec3 Up()
    {
        return glm::vec3(0, 1, 0);
    }
};
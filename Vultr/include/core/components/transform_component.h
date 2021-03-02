#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <ecs/entity/entity.hpp>

struct TransformComponent
{
    TransformComponent() = default;

    static TransformComponent Create(const glm::vec3 &p_position = glm::vec3(0, 0,
                                                                             0),
                                     const glm::quat &p_rotation = glm::quat(1, 0, 0,
                                                                             0),
                                     const glm::vec3 &p_scale = glm::vec3(1, 1, 1))
    {
        TransformComponent component = TransformComponent();
        component.position = p_position;
        component.rotation = p_rotation;
        component.scale = p_scale;
        return component;
    }

    glm::vec3 position = glm::vec3(0, 0, 0);
    glm::quat rotation = glm::quat(1, 0, 0, 0);
    glm::vec3 scale = glm::vec3(1, 1, 1);

    inline glm::vec3 Forward() const
    {
        return rotation * glm::vec3(0, 0, -1);
    }

    inline glm::vec3 Right() const
    {
        return rotation * glm::vec3(1, 0, 0);
    }
    inline glm::vec3 Up() const
    {
        return glm::vec3(0, 1, 0);
    }

    inline glm::mat4 Matrix() const
    {
        glm::mat4 scaling_matrix = glm::scale(scale);
        glm::mat4 rotation_matrix = glm::toMat4(rotation);
        glm::mat4 translate_matrix = glm::translate(position);
        return translate_matrix * rotation_matrix * scaling_matrix;
    }

    inline glm::mat4 GetViewMatrix() const
    {
        return glm::lookAt(position, position + Forward(), glm::vec3(0, 1, 0));
    }

    template <class Archive> void serialize(Archive &ar)
    {
        ar(position.x, position.y, position.z, rotation.x, rotation.y, rotation.z,
           rotation.w, scale.x, scale.y, scale.z);
    }
};

#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <fundamental/types.h>

namespace Vultr
{
    struct TransformComponent
    {
        TransformComponent() = default;

        static TransformComponent Create(const Vec3 &p_position = Vec3(0, 0, 0), const Quat &p_rotation = Quat(1, 0, 0, 0), const Vec3 &p_scale = Vec3(1, 1, 1))
        {
            TransformComponent component = TransformComponent();
            component.position = p_position;
            component.rotation = p_rotation;
            component.scale = p_scale;
            return component;
        }

        Vec3 position = Vec3(0, 0, 0);
        Quat rotation = Quat(1, 0, 0, 0);
        Vec3 scale = Vec3(1, 1, 1);

        inline Vec3 Forward() const
        {
            return rotation * Vec3(0, 0, -1);
        }

        inline Vec3 Right() const
        {
            return rotation * Vec3(1, 0, 0);
        }
        inline Vec3 Up() const
        {
            return rotation * Vec3(0, 1, 0);
        }

        inline Mat4 Matrix() const
        {
            Mat4 scaling_matrix = glm::scale(scale);
            Mat4 rotation_matrix = glm::toMat4(rotation);
            Mat4 translate_matrix = glm::translate(position);
            return translate_matrix * rotation_matrix * scaling_matrix;
        }

        inline Mat4 GetViewMatrix() const
        {
            return glm::lookAt(position, position + Forward(), Vec3(0, 1, 0));
        }
    };
} // namespace Vultr

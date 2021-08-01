#pragma once
#include <types/types.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <fundamental/types.h>

namespace Vultr
{

    struct CameraComponent
    {
        static CameraComponent Create()
        {
            CameraComponent component = CameraComponent();
            return component;
        }

        bool enabled = true;

        f32 fov = 45.0f;
        f32 znear = 0.1f;
        f32 zfar = 100.0f;
        f32 exposure = 1.0f;
        f32 bloom_intensity = 1.0f;
        u16 bloom_quality = 10;

        bool gamma_correction = true;

        Mat4 GetProjectionMatrix(float width, float height) const
        {
            return glm::perspective(fov, width / height, znear, zfar);
        }
    };
} // namespace Vultr

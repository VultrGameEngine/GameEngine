#pragma once
#include <types/types.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

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

    bool gamma_correction = true;

    glm::mat4 GetProjectionMatrix(float width, float height) const
    {
        return glm::perspective(fov, width / height, znear, zfar);
    }
};

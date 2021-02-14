#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

struct CameraComponent
{
    static CameraComponent &Create()
    {
        CameraComponent *component = new CameraComponent();
        return *component;
    }

    bool enabled = true;

    float fov = 45.0f;
    float znear = 0.1f;
    float zfar = 100.0f;

    unsigned int skybox = 0;

    glm::mat4 GetProjectionMatrix(float width, float height) const
    {
        return glm::perspective(fov, width / height, znear, zfar);
    }
};

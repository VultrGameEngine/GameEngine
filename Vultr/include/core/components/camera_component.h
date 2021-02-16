#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <memory>

struct CameraComponent
{
    static std::shared_ptr<CameraComponent> Create()
    {
        std::shared_ptr<CameraComponent> component =
            std::make_shared<CameraComponent>();
        return component;
    }

    bool enabled = true;

    float fov = 45.0f;
    float znear = 0.1f;
    float zfar = 100.0f;

    glm::mat4 GetProjectionMatrix(float width, float height) const
    {
        return glm::perspective(fov, width / height, znear, zfar);
    }

    template <class Archive> void serialize(Archive &ar)
    {
        ar(enabled, fov, znear, zfar);
    }
};

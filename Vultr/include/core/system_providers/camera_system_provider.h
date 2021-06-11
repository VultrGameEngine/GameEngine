#pragma once
#include <core/components/camera_component.h>
#include <core/components/controller_component.h>
#include <core/components/transform_component.h>
#include <engine.hpp>

namespace Vultr
{
    namespace CameraSystem
    {
        struct Component : public SystemProvider
        {
            // Member variables for state
            Entity camera = INVALID_ENTITY;
            struct Camera
            {
                TransformComponent transform_component;
                CameraComponent camera_component;
                ControllerComponent controller_component;
            } scene_camera;
        };

        Component &get_provider();

    } // namespace CameraSystem

    template <>
    inline const char *get_struct_name<CameraSystem::Component>()
    {
        return "CameraSystem";
    }
} // namespace Vultr

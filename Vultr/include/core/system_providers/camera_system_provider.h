#pragma once
#include <core/components/camera_component.h>
#include <core/components/controller_component.h>
#include <core/components/transform_component.h>
#include <ecs/entity/entity.hpp>
#include <ecs/system/system_provider.hpp>

namespace Vultr
{
class CameraSystemProvider : public SystemProvider
{
  public:
    // Singleton pattern for all providers
    static CameraSystemProvider &Get()
    {
        static CameraSystemProvider instance;
        return instance;
    }

    // Member variables for state
    Entity m_camera = Entity(-1);
    struct Camera
    {
        TransformComponent transform_component;
        CameraComponent camera_component;
        ControllerComponent controller_component;
    } m_scene_camera;

    void Reset() override
    {
        m_camera = Entity(-1);
    }
};

} // namespace Vultr

#pragma once
#include <core/components/camera_component.h>
#include <core/components/transform_component.h>
#include <ecs/entity/entity.hpp>
#include <ecs/system/system_provider.hpp>

namespace Brick3D
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
    Entity m_camera;
    struct Camera
    {
        TransformComponent transform_component;
        CameraComponent camera_component;
    } m_scene_camera;
};

} // namespace Brick3D

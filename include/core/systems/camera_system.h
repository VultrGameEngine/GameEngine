// Camera system handles updating the main camera's matricies to match the
// transform

#pragma once
#include <core/components/camera_component.h>
#include <core/components/transform_component.h>
#include <core/system_providers/camera_system_provider.h>
#include <ecs/system/system.hpp>
#include <ecs/world/world.hpp>
#include <memory>

namespace Brick3D
{
class CameraSystem : public System
{
  public:
    static void RegisterSystem();

  protected:
    void OnCreateEntity(Entity entity) override;
    void OnDestroyEntity(Entity entity) override;
    SystemProvider &GetProvider() override
    {
        return CameraSystemProvider::Get();
    }
};
} // namespace Brick3D

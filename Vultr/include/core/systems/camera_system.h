// Camera system handles updating the main camera's matricies to match the
// transform

#pragma once
#include <core/components/transform_component.h>
#include <core/system_providers/camera_system_provider.h>
#include <memory>

namespace Vultr
{
class CameraSystem
{
  public:
    static void RegisterSystem();
    static void OnCreateEntity(Entity entity);
    static void OnDestroyEntity(Entity entity);

  private:
    static SystemProvider &GetProvider()
    {
        std::shared_ptr<CameraSystemProvider> provider = CameraSystemProvider::Get();
        return *provider;
    }
};
} // namespace Vultr

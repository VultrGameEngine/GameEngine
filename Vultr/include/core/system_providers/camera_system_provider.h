#pragma once
#include <core/components/camera_component.h>
#include <core/components/controller_component.h>
#include <core/components/transform_component.h>
#include <ecs/system/system_provider.hpp>
#include <engine.hpp>

namespace Vultr
{
class CameraSystemProvider : public SystemProvider
{
  public:
    // Singleton pattern for all providers
    static std::shared_ptr<CameraSystemProvider> Get()
    {
        return Engine::GetSystemProvider<CameraSystemProvider>();
    }

    // Member variables for state
    Entity m_camera = Entity(-1);
    struct Camera
    {
        TransformComponent transform_component;
        CameraComponent camera_component;
        ControllerComponent controller_component;
    } m_scene_camera;

  protected:
    void OnDestroyEntity(Entity entity) override;
    void OnCreateEntity(Entity entity) override;
};

} // namespace Vultr
VultrRegisterSystemProvider(Vultr::CameraSystemProvider)

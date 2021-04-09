#include <core/components/camera_component.h>
#include <core/systems/camera_system.h>
#include <ecs/world/world.hpp>
#include <engine.hpp>

namespace Vultr
{

void CameraSystem::OnCreateEntity(Entity entity)
{
    auto &camera_component = entity.GetComponent<CameraComponent>();
    if (camera_component.enabled)
    {
        CameraSystemProvider::Get()->m_camera = entity;
    }
}

void CameraSystem::OnDestroyEntity(Entity entity)
{
    CameraSystemProvider &provider = *CameraSystemProvider::Get();
    if (provider.m_camera == entity)
    {
        provider.m_camera = -1;
        for (Entity camera_entity : provider.entities)
        {
            auto &camera_component = camera_entity.GetComponent<CameraComponent>();
            if (camera_component.enabled)
            {
                provider.m_camera = entity;
                break;
            }
        }
    }
}

void CameraSystem::RegisterSystem()
{
    Signature signature;
    signature.set(Engine::GetComponentRegistry().GetComponentType<CameraComponent>(),
                  true);
    signature.set(
        Engine::GetComponentRegistry().GetComponentType<TransformComponent>(), true);
    Engine::RegisterGlobalSystem<CameraSystemProvider>(signature);
}
} // namespace Vultr

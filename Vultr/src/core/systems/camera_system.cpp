#include <core/components/camera_component.h>
#include <core/systems/camera_system.h>
#include <ecs/world/world.hpp>

namespace Vultr
{

void CameraSystem::OnCreateEntity(Entity entity)
{
    auto &camera_component = World::GetComponent<CameraComponent>(entity);
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
            auto &camera_component =
                World::GetComponent<CameraComponent>(camera_entity);
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
    signature.set(World::GetComponentType<CameraComponent>(), true);
    signature.set(World::GetComponentType<TransformComponent>(), true);
    World::RegisterSystem<CameraSystemProvider>(signature);
}
} // namespace Vultr

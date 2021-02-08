#include <core/components/light_component.h>
#include <core/components/transform_component.h>
#include <core/systems/light_system.h>
#include <ecs/world/world.hpp>

namespace Brick3D
{
void LightSystem::Update()
{
    LightSystemProvider &provider = LightSystemProvider::Get();
    for (Entity entity : provider.entities)
    {
        provider.light = entity;
        return;
    }
    provider.light = -1;
}

void LightSystem::OnCreateEntity(Entity entity)
{
    std::cout << "New light" << std::endl;
}

void LightSystem::RegisterSystem()
{
    LightSystemProvider &provider = LightSystemProvider::Get();
    provider.signature.set(World::GetComponentType<LightComponent>(), true);
    provider.signature.set(World::GetComponentType<TransformComponent>(), true);
    World::RegisterSystem<LightSystem>(provider.signature);
}
} // namespace Brick3D

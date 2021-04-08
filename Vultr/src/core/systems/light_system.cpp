#include <core/components/light_component.h>
#include <core/components/transform_component.h>
#include <core/systems/light_system.h>
#include <ecs/world/world.hpp>
#include <engine.hpp>

namespace Vultr
{

void LightSystem::OnCreateEntity(Entity entity)
{
    LightSystemProvider &provider = *LightSystemProvider::Get();
    provider.light = entity;
}

void LightSystem::RegisterSystem()
{
    Signature signature;
    signature.set(Engine::GetComponentRegistry().GetComponentType<LightComponent>(),
                  true);
    signature.set(
        Engine::GetComponentRegistry().GetComponentType<TransformComponent>(), true);
    Engine::RegisterGlobalSystem<LightSystemProvider>(signature);
}
} // namespace Vultr

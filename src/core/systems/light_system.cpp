#include "../../../include/core/components/transform_component.h"
#include "../../../include/core/components/light_component.h"
#include "../../../include/core/systems/light_system.h"
#include "../../../include/ecs/world/world.hpp"

void LightSystem::Update()
{
    Signature light_signature;
    light_signature.set(World::GetComponentType<LightComponent>(), true);
    light_signature.set(World::GetComponentType<TransformComponent>(), true);
    for (Entity entity : World::GetEntities(light_signature))
    {
        light = entity;
        return;
    }
    light = -1;
}

std::shared_ptr<LightSystem> LightSystem::Get()
{
    static std::shared_ptr<LightSystem> instance;
    if (instance == nullptr)
    {
        instance = RegisterSystem();
    }
    return instance;
}

std::shared_ptr<LightSystem> LightSystem::RegisterSystem()
{
    std::shared_ptr<LightSystem> ptr = World::RegisterSystem<LightSystem>();
    return ptr;
}

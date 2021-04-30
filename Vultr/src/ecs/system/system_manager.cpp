#include <ecs/entity/entity.hpp>
#include <ecs/system/system_manager.hpp>

namespace Vultr::SystemManager
{
void EntityDestroyed(SystemManager &manager, Entity entity)
{
    // Erase a destroyed entity from all system lists
    for (auto const &pair : manager.system_providers)
    {
        auto const &system = pair.second;

        system->DestroyEntity(entity);
    }
}

void EntitySignatureChanged(SystemManager &manager, Entity entity,
                            Signature entity_signature)
{
    // Notify each system that an entity's signature has changed
    for (auto const &pair : manager.system_providers)
    {
        auto const &type = pair.first;
        auto const &system = pair.second;
        auto const &system_signature = manager.signatures[type];

        // If entity signature matches system signature
        if (system->Match(entity_signature))
        {
            // Insert into set
            system->CreateEntity(entity);
        }
        // Entity signature does not match system signature
        else
        {
            // Erase from set
            system->DestroyEntity(entity);
        }
    }
}
} // namespace Vultr::SystemManager

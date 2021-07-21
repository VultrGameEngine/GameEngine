#include <ecs/entity/entity.hpp>
#include <ecs/system/system_manager.hpp>

namespace Vultr
{
    void system_manager_entity_destroyed(Engine *e, SystemManager &m, Entity entity)
    {
        // Erase a destroyed entity from all system lists
        for (auto const &pair : m.system_providers)
        {
            auto const &system = pair.second;
            system->entities.erase(entity);
            if (system->on_destroy_entity != nullptr)
            {
                system->on_destroy_entity(e, entity);
            }
        }
    }

    void system_manager_entity_signature_changed(Engine *e, SystemManager &m, Entity entity, Signature entity_signature)
    {
        // Notify each system that an entity's signature has changed
        for (auto &pair : m.system_providers)
        {
            auto &type = pair.first;
            auto &system = pair.second;
            auto &system_signature = pair.second->signature;

            system_manager_entity_signature_changed_in_system(e, system, entity, entity_signature);
        }
    }

    void system_manager_entity_signature_changed_in_system(Engine *e, SystemProvider *system, Entity entity, Signature entity_signature)
    {
        bool match_signature;
        if (system->match_signature != nullptr)
        {
            match_signature = system->match_signature(e, entity_signature);
        }
        else
        {
            match_signature = signature_contains(system->signature, entity_signature);
        }

        // If entity signature matches system signature
        if (match_signature)
        {
            // Insert into set
            system_provider_on_create_entity(e, *system, entity);
        }
        // Entity signature does not match system signature
        else
        {
            // Erase from set
            system_provider_on_destroy_entity(e, *system, entity);
        }
    }

} // namespace Vultr

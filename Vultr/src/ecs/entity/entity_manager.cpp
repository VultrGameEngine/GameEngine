#include <ecs/entity/entity_manager.hpp>

namespace Vultr
{
    EntityManager init_entity_manager()
    {
        EntityManager manager = {};
        for (Entity entity = 1; entity < MAX_ENTITIES; ++entity)
        {
            manager.available_entities.push(entity);
        }
        return manager;
    }

    Entity entity_manager_create_entity(EntityManager &manager)
    {

        assert(manager.living_entity_count < MAX_ENTITIES &&
               "Too many entities in existence");

        // Get the first ID from the queue and use that
        Entity id = manager.available_entities.front();

        // Remove this ID from the available entities, since we are now gonna use it
        manager.available_entities.pop();

        // Add the entity to the living entities
        manager.living_entites.insert(id);

        // Increase the number of living entities
        ++manager.living_entity_count;

        return Entity(id);
    }

    void entity_manager_destroy_entity(EntityManager &manager, Entity entity)
    {

        assert(entity < MAX_ENTITIES && "Entity out of range");

        // We don't want the signature anymore
        manager.signatures[entity].reset();

        // Put the destroyed ID at the back of the queue
        manager.available_entities.push(entity);

        // Remove the entity id from the living entities
        manager.living_entites.erase(entity);

        // Decrease the number of living entities
        --manager.living_entity_count;
    }

    // Set the components of an entity
    void entity_manager_set_signature(EntityManager &manager, Entity entity,
                                      Signature signature)
    {

        assert(entity < MAX_ENTITIES && "Entity out of range");

        // Put the signature into the array
        manager.signatures[entity] = signature;
    }

    Signature entity_manager_get_signature(EntityManager &manager, Entity entity)
    {

        assert(entity < MAX_ENTITIES && "Entity out of range");

        // Get the entity signature from the array
        return manager.signatures[entity];
    }

    std::set<Entity> entity_manager_get_entities(EntityManager &manager,
                                                 Signature signature)
    {
        std::set<Entity> entities{};

        for (Entity entity = 0; entity < manager.signatures.size(); ++entity)
        {
            if ((manager.signatures[entity] & signature) == signature)
            {
                entities.insert(Entity(entity));
            }
        }
        return entities;
    }
} // namespace Vultr

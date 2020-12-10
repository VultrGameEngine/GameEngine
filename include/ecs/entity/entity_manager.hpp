
// In charge of distributing entity IDs and keeping record of which IDs are in use and which are not
//
// The queue will contain every valid entity ID up to the MAX_ENTITIES
// When an entity is created, it will use the ID at the front of the queue as its own
// When an entity is destroyed, it will put its own id at the back of the queue

#include "entity.hpp"
#include "../component/component.hpp"
#include <queue>
#include <array>
#include <assert.h>

class EntityManager
{
public:
    EntityManager()
    {

        for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
        {
            available_entities.push(entity);
        }
    }

    Entity CreateEntity()
    {

        assert(living_entity_count < MAX_ENTITIES && "Too many entities in existence");

        // Get the first ID from the queue and use that
        Entity id = available_entities.front();

        // Remove this ID from the available entities, since we are now gonna use it
        available_entities.pop();

        // Increase the number of living entities
        ++living_entity_count;

        return id;
    }

    void DestroyEntity(Entity entity)
    {

        assert(entity < MAX_ENTITIES && "Entity out of range");

        // We don't want the signature anymore
        signatures[entity].reset();

        // Put the destroyed ID at the back of the queue
        available_entities.push(entity);
        --living_entity_count;
    }

    // Set the components of an entity
    void SetSignature(Entity entity, Signature signature)
    {

        assert(entity < MAX_ENTITIES && "Entity out of range");

        // Put the signature into the array
        signatures[entity] = signature;
    }
    Signature GetSignature(Entity entity)
    {

        assert(entity < MAX_ENTITIES && "Entity out of range");

        // Get the entity signature from the array
        return signatures[entity];
    }

    std::set<Entity> GetEntities(Signature signature)
    {
        std::set<Entity> entities{};

        for (Entity entity = 0; entity < signatures.size(); ++entity)
        {
            if ((signatures[entity] & signature) == signature)
            {
                entities.insert(entity);
            }
        }
        return entities;
    }

private:
    // Queue of unused entity IDs
    std::queue<Entity> available_entities{};

    // Array of signatures where the index corresponds to the entity ID
    std::array<Signature, MAX_ENTITIES> signatures{};

    // Total living entities
    uint32_t living_entity_count{};
};
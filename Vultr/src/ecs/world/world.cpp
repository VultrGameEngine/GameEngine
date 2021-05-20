#include <ecs/world/world.hpp>
#include <ecs/world/internal_world.hpp>
#include <helpers/file.h>
#include <json/json.hpp>
#include <engine.hpp>

namespace Vultr
{
    World *new_world()
    {
        World *world = new InternalWorld();
        return world;
    }

    World *load_world(const File &file)
    {
    }

    void save_world(World *_world, const File &out)
    {
        InternalWorld *world = static_cast<InternalWorld *>(_world);
        json component_manager_json;
        component_manager_to_json(component_manager_json, world_get_component_manager(world), engine_global().component_registry);
    }

    Entity create_entity(World *world)
    {
        return entity_manager_create_entity(world->entity_manager);
    }

    void destroy_entity(World *world, Entity entity)
    {
        entity_manager_destroy_entity(world->entity_manager, entity);
    }

    std::set<Entity> get_entities(World *world, Signature signature)
    {
        return entity_manager_get_entities(world->entity_manager, signature);
    }

    Signature get_entity_signature(World *world, Entity entity)
    {
        return entity_manager_get_signature(world->entity_manager, entity);
    }

    // Getters for World
    EntityManager &world_get_entity_manager(World *world)
    {
        return world->entity_manager;
    }
    ComponentManager &world_get_component_manager(World *world)
    {
        return world->component_manager;
    }
    SystemManager &world_get_system_manager(World *world)
    {
        return world->system_manager;
    }
} // namespace Vultr

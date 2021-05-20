// Bundles the EntityManager, ComponentManager, and SystemManager into one World
#pragma once
#define GLFW_INCLUDE_NONE
#include <ecs/entity/entity.hpp>
#include <ecs/component/component.hpp>
#include <ecs/entity/entity_manager.hpp>
#include <ecs/component/component_manager.hpp>
#include <ecs/system/system_manager.hpp>
#include <set>
#include <helpers/file.h>

namespace Vultr
{
    // Opaque type World
    typedef struct InternalWorld World;

    // Create a new World
    World *new_world();

    void save_world(World *world, const File &out);

    // void ExportWorld(const std::string &path, std::shared_ptr<World> world);

    // void ExportWorldEditor(const std::string &path, std::shared_ptr<World> world);

    // World ImportWorld(const std::string &path);

    World *load_world(const File &file);

    // Entity methods, just wrappers around existing methods
    Entity create_entity(World *world);
    void destroy_entity(World *world, Entity entity);
    std::set<Entity> get_entities(World *world, Signature signature);
    Signature get_entity_signature(World *world, Entity entity);

    // Getters for World
    EntityManager &world_get_entity_manager(World *world);
    ComponentManager &world_get_component_manager(World *world);
    SystemManager &world_get_system_manager(World *world);
} // namespace Vultr

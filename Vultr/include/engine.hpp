#pragma once
#include <ecs/component/component_registry.hpp>
#include <ecs/world/world.hpp>

namespace Vultr
{
    struct Game;
    struct Engine
    {
        GLFWwindow *window;
        bool should_close = false;

        bool debug;
        Game *game;
        World *current_world;
        ComponentRegistry component_registry;
        SystemManager system_manager;
    };

    Engine &engine_global();

    inline World *get_current_world();

    void engine_init(Engine &e, bool debug);
    void engine_load_game(Engine &e, const std::string &path);
    void engine_load_game(Engine &e, Game *game);
    void engine_register_default_components(Engine &e);
    void engine_init_default_systems(Engine &e);
    void engine_init_game(Engine &e);
    void engine_update_game(Engine &e, float &last_time);
    double engine_get_time_elapsed(Engine &e);

    template <typename T>
    void register_component()
    {
        component_registry_register_component<T>(engine_global().component_registry);
    }

    template <typename T>
    ComponentType get_component_type()
    {
        component_registry_get_component_type<T>(engine_global().component_registry);
    }

    template <typename T>
    bool is_component_registered()
    {
        ComponentType type = component_registry_get_component_type<T>(
            engine_global().component_registry);
        return component_registry_is_component_registered(
            engine_global().component_registry, type);
    }

    template <typename T>
    T *engine_register_global_system(Engine &e, Signature signature)
    {
        return system_manager_register_system<T>(e.system_manager, signature);
    }

    template <typename T>
    void engine_deregister_global_system(Engine &e)
    {
        return system_manager_deregister_system<T>(e.system_manager);
    }

    template <typename T>
    std::shared_ptr<T> engine_get_global_system_provider(Engine &e)
    {
        return system_manager_get_system_provider<T>(e.system_manager);
    }

    template <typename T>
    T &entity_get_component(Entity entity)
    {
        World *world = get_current_world();
        assert(world != nullptr &&
               "Cannot get component because world does not exist!");
        return component_manager_get_component_array<T>(
                   world_get_component_manager(world))
            ->GetData(entity);
    }

    template <typename T>
    void entity_add_component(Entity entity, T component)
    {
        World *world = get_current_world();
        assert(world != nullptr &&
               "World does not exist! Make sure you create a world "
               "before trying to add a component to an entity");
        component_manager_get_component_array<T>(world_get_component_manager(world))
            ->InsertData(entity, component);

        auto signature = get_entity_signature(world, entity);
        signature.set(hash_struct<T>(), true);
        system_manager_entity_signature_changed(world_get_system_manager(world),
                                                entity, signature);

        system_manager_entity_signature_changed(engine_global().system_manager,
                                                entity, signature);
        entity_manager_set_signature(world_get_entity_manager(world), entity,
                                     signature);
    }

    template <typename T>
    void entity_remove_component(Entity entity)
    {
        World *world = get_current_world();
        assert(world != nullptr &&
               "World does not exist! Make sure you create a world "
               "before trying to remove a component to an entity");
        auto signature = get_entity_signature(world, entity);
        signature.set(get_component_type<T>(), false);
        system_manager_entity_signature_changed(world_get_system_manager(world),
                                                entity, signature);
        system_manager_entity_signature_changed(engine_global().system_manager,
                                                entity, signature);
        entity_manager_set_signature(world_get_entity_manager(world), entity,
                                     signature);

        component_manager_get_component_array<T>(world_get_component_manager(world))
            ->RemoveData(entity);
    }

    template <typename T>
    bool entity_has_component(Entity entity)
    {
        World *world = get_current_world();
        assert(world != nullptr &&
               "Cannot get component because world does not exist!");
        return component_manager_get_component_array<T>(
                   world_get_component_manager(world))
            ->HasComponent(entity);
    }

    template <typename T>
    T *entity_get_component_unsafe(Entity entity)
    {
        World *world = get_current_world();
        assert(world != nullptr &&
               "Cannot get component because world does not exist!");
        return component_manager_get_component_array<T>(
                   world_get_component_manager(world))
            ->GetDataUnsafe(entity);
    }
} // namespace Vultr

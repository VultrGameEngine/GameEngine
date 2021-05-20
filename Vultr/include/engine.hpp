#pragma once
#include <ecs/component/component_registry.hpp>
#include <platform/window/window.h>
#include <ecs/world/world.hpp>
#include <game.hpp>
#include <GLFW/glfw3.h>

namespace Vultr
{
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

    World *get_current_world();
    void change_world(World *);

    void engine_init(Engine &e, bool debug);
    void engine_load_game(Engine &e, const char *path);
    void engine_load_game(Engine &e, Game *game);
    void engine_register_default_components(Engine &e);
    void engine_init_default_systems(Engine &e);
    void engine_init_game(Engine &e);
    void engine_update_game(Engine &e, float &last_time);
    double engine_get_time_elapsed(Engine &e);

#define WORLD_DOESNT_EXIST_ERROR(function_name) "Cannot call " #function_name " because world does not exist! Make sure you create a world before calling this method."

    template <typename T>
    ComponentType get_component_type()
    {
        return component_registry_get_component_type<T>(engine_global().component_registry);
    }

    template <typename T>
    T &entity_get_component(Entity entity)
    {
        World *world = get_current_world();
        assert(world != nullptr && WORLD_DOESNT_EXIST_ERROR(entity_get_component));
        return component_manager_get_component_array<T>(world_get_component_manager(world), get_component_type<T>())->GetData(entity);
    }

    template <typename T>
    void entity_add_component(Entity entity, T component)
    {
        World *world = get_current_world();
        assert(world != nullptr && WORLD_DOESNT_EXIST_ERROR(entity_add_component));
        component_manager_get_component_array<T>(world_get_component_manager(world), get_component_type<T>())->InsertData(entity, component);

        auto signature = get_entity_signature(world, entity);
        signature.set(get_component_type<T>(), true);
        system_manager_entity_signature_changed(world_get_system_manager(world), entity, signature);

        system_manager_entity_signature_changed(engine_global().system_manager, entity, signature);
        entity_manager_set_signature(world_get_entity_manager(world), entity, signature);
    }

    template <typename T>
    void entity_remove_component(Entity entity)
    {
        World *world = get_current_world();
        assert(world != nullptr && WORLD_DOESNT_EXIST_ERROR(entity_remove_component));
        auto signature = get_entity_signature(world, entity);
        signature.set(get_component_type<T>(), false);
        system_manager_entity_signature_changed(world_get_system_manager(world), entity, signature);
        system_manager_entity_signature_changed(engine_global().system_manager, entity, signature);
        entity_manager_set_signature(world_get_entity_manager(world), entity, signature);

        component_manager_get_component_array<T>(world_get_component_manager(world), get_component_type<T>())->RemoveData(entity);
    }

    template <typename T>
    bool entity_has_component(Entity entity)
    {
        World *world = get_current_world();
        assert(world != nullptr && WORLD_DOESNT_EXIST_ERROR(entity_has_component));
        return component_manager_get_component_array<T>(world_get_component_manager(world), get_component_type<T>())->HasComponent(entity);
    }

    template <typename T>
    T *entity_get_component_unsafe(Entity entity)
    {
        World *world = get_current_world();
        assert(world != nullptr && WORLD_DOESNT_EXIST_ERROR(entity_get_component_unsafe));
        return component_manager_get_component_array<T>(world_get_component_manager(world), get_component_type<T>())->GetDataUnsafe(entity);
    }

    Signature entity_get_signature(Entity entity);

    template <typename T>
    void register_component()
    {
        component_registry_register_component<T>(engine_global().component_registry, [](Entity entity) { entity_add_component(entity, T::Create()); });
        component_manager_get_component_array<T>(world_get_component_manager(get_current_world()), get_component_type<T>());
    }

    template <typename T>
    bool is_component_registered()
    {
        ComponentType type = component_registry_get_component_type<T>(engine_global().component_registry);
        return component_registry_is_component_registered(engine_global().component_registry, type);
    }

    template <typename T>
    T *world_register_system(Signature signature, OnCreateEntity on_create_entity = nullptr, OnDestroyEntity on_destroy_entity = nullptr, MatchSignature match_signature = nullptr)
    {
        return system_manager_register_system<T>(world_get_system_manager(get_current_world()), signature, on_create_entity, on_destroy_entity, match_signature);
    }

    template <typename T>
    T *register_global_system(Signature signature, OnCreateEntity on_create_entity = nullptr, OnDestroyEntity on_destroy_entity = nullptr, MatchSignature match_signature = nullptr)
    {
        return system_manager_register_system<T>(engine_global().system_manager, signature, on_create_entity, on_destroy_entity, match_signature);
    }

    template <typename T>
    void deregister_global_system()
    {
        return system_manager_deregister_system<T>(engine_global().system_manager);
    }

    template <typename T>
    T *get_global_system_provider()
    {
        return system_manager_get_system_provider<T>(engine_global().system_manager);
    }

} // namespace Vultr

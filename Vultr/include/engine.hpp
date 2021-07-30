#pragma once
#include <ecs/component/component_registry.hpp>
#include <platform/window/window.h>
#include <ecs/world/world.hpp>
#include <game.hpp>
#include <GLFW/glfw3.h>

namespace Vultr
{
    struct EditEvent;
    typedef void (*OnEdit)(void *editor, EditEvent *);
    struct Engine
    {
        GLFWwindow *window;
        bool should_close = false;

        bool debug;
        Game *game;
        World *current_world;
        ComponentRegistry component_registry;
        SystemManager system_manager;

        // EDITOR ONLY
        OnEdit on_edit = nullptr;
        void *editor = nullptr;
        void *dll = nullptr;
    };

    World *get_current_world(Engine *e);
    void change_world(Engine *e, World *new_world);

    Engine *get_engine(GLFWwindow *window);
    void add_editor(Engine *e, void *editor);

    void engine_init(Engine *e, bool debug);
    void engine_load_game(Engine *e, const char *path);
    void engine_load_game(Engine *e, Game *game);

    void engine_detach_game(Engine *e);
    void engine_flush_game(Engine *e);

    void engine_register_default_components(Engine *e);
    void engine_init_default_systems(Engine *e);
    void engine_init_game(Engine *e);
    UpdateTick engine_update_game(Engine *e, float &last_time, bool play);
    double engine_get_time_elapsed(Engine *e);

    void engine_send_update_event(Engine *e, EditEvent *event);

#define WORLD_DOESNT_EXIST_ERROR(function_name) "Cannot call " #function_name " because world does not exist! Make sure you create a world before calling this method."

    template <typename T>
    ComponentType get_component_type(Engine *e)
    {
        return component_registry_get_component_type<T>(e->component_registry);
    }

    template <typename T>
    T &entity_get_component(Engine *e, Entity entity)
    {
        World *world = get_current_world(e);
        assert(world != nullptr && WORLD_DOESNT_EXIST_ERROR(entity_get_component));
        return component_manager_get_component_array<T>(world_get_component_manager(world), get_component_type<T>(e))->GetData(entity);
    }

    template <typename... Components>
    std::tuple<Components &...> entity_get_components(Engine *e, Entity entity)
    {
        return std::tuple<Components &...>((entity_get_component<Components>(e, entity))...);
    }

    template <typename T>
    void entity_add_component(Engine *e, Entity entity, T component)
    {
        World *world = get_current_world(e);
        assert(world != nullptr && WORLD_DOESNT_EXIST_ERROR(entity_add_component));
        component_manager_get_component_array<T>(world_get_component_manager(world), get_component_type<T>(e))->InsertData(entity, component);

        auto signature = get_entity_signature(world, entity);
        signature.set(get_component_type<T>(e), true);
        system_manager_entity_signature_changed(e, world_get_system_manager(world), entity, signature);

        system_manager_entity_signature_changed(e, e->system_manager, entity, signature);
        entity_manager_set_signature(world_get_entity_manager(world), entity, signature);
    }

    template <typename T>
    void entity_remove_component(Engine *e, Entity entity)
    {
        World *world = get_current_world(e);
        assert(world != nullptr && WORLD_DOESNT_EXIST_ERROR(entity_remove_component));
        auto signature = get_entity_signature(world, entity);
        signature.set(get_component_type<T>(e), false);
        system_manager_entity_signature_changed(e, world_get_system_manager(world), entity, signature);
        system_manager_entity_signature_changed(e, e->system_manager, entity, signature);
        entity_manager_set_signature(world_get_entity_manager(world), entity, signature);

        component_manager_get_component_array<T>(world_get_component_manager(world), get_component_type<T>(e))->RemoveData(entity);
    }

    template <typename T>
    bool entity_has_component(Engine *e, Entity entity)
    {
        World *world = get_current_world(e);
        assert(world != nullptr && WORLD_DOESNT_EXIST_ERROR(entity_has_component));
        return component_manager_get_component_array<T>(world_get_component_manager(world), get_component_type<T>(e))->HasData(entity);
    }

    template <typename T>
    T *entity_get_component_unsafe(Engine *e, Entity entity)
    {
        World *world = get_current_world(e);
        assert(world != nullptr && WORLD_DOESNT_EXIST_ERROR(entity_get_component_unsafe));
        return component_manager_get_component_array<T>(world_get_component_manager(world), get_component_type<T>(e))->GetDataUnsafe(entity);
    }

    void destroy_entity(Engine *e, Entity entity);

    Signature entity_get_signature(Engine *e, Entity entity);

    template <typename T>
    void register_component(Engine *e)
    {
        component_registry_register_component<T>(
            e->component_registry, [](Engine *e, Entity entity) { entity_add_component(e, entity, T::Create()); }, true);
    }

    template <typename T>
    bool is_component_registered(Engine *e)
    {
        ComponentType type = component_registry_get_component_type<T>(e->component_registry);
        return component_registry_is_component_registered(e->component_registry, type);
    }

    template <typename T>
    T *world_register_system(Engine *e, Signature signature, OnCreateEntity on_create_entity = nullptr, OnDestroyEntity on_destroy_entity = nullptr, MatchSignature match_signature = nullptr)
    {
        T *res = system_manager_register_system<T>(world_get_system_manager(get_current_world(e)), signature, on_create_entity, on_destroy_entity, match_signature);
        EntityManager &entity_manager = world_get_entity_manager(get_current_world(e));
        for (Entity entity : entity_manager.living_entites)
        {
            system_manager_entity_signature_changed_in_system(e, res, entity, entity_manager_get_signature(entity_manager, entity));
        }
        return res;
    }

    template <typename T>
    T *register_global_system(Engine *e, Signature signature, OnCreateEntity on_create_entity = nullptr, OnDestroyEntity on_destroy_entity = nullptr, MatchSignature match_signature = nullptr)
    {
        return system_manager_register_system<T>(e->system_manager, signature, on_create_entity, on_destroy_entity, match_signature);
    }

    template <typename T>
    void deregister_global_system(Engine *e)
    {
        return system_manager_deregister_system<T>(e->system_manager);
    }

    template <typename T>
    T *world_get_system_provider(Engine *e)
    {
        World *world = get_current_world(e);
        assert(world != nullptr && WORLD_DOESNT_EXIST_ERROR(world_get_system_provider));
        return system_manager_get_system_provider<T>(world_get_system_manager(world));
    }

    template <typename T>
    T *get_global_system_provider(Engine *e)
    {
        return system_manager_get_system_provider<T>(e->system_manager);
    }

} // namespace Vultr

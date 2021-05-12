#pragma once
#include "Game.hpp"
#include <ecs/component/component_registry.hpp>
#include <ecs/system/system_manager.hpp>

namespace Vultr
{
    struct World;
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

    void engine_init(Engine &e, bool debug);
    void engine_load_game(Engine &e, const std::string &path);
    void engine_load_game(Engine &e, Game *game);
    void engine_register_default_components(Engine &e);
    void engine_init_default_systems(Engine &e);
    void engine_init_game(Engine &e);
    void engine_update_game(Engine &e, float &last_time);
    double engine_get_time_elapsed(Engine &e);

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
} // namespace Vultr

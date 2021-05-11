#pragma once
#include "Game.hpp"
#include <ecs/component/component_registry.hpp>
#include <ecs/system/system_manager.hpp>
#include <GLFW/glfw3.h>

namespace Vultr
{
    struct Engine
    {
        GLFWwindow *window;
        bool should_close = false;

        bool debug;
        Game *game;
        ComponentRegistry component_registry;
        SystemManager system_manager;
    };

    Engine &get_engine();
    void Init(bool debug);
    void LoadGame(const std::string &path);
    void LoadGame(Game *game);
    void RegisterComponents();
    void InitSystems();
    void InitGame();
    void UpdateGame(float &t);
    void Destroy();
    double GetElapsedTime();

    // Component methods
    template <typename T>
    void RegisterComponent(bool inspector_available = true)
    {
        Get().component_registry.RegisterComponent<T>(inspector_available);
    }

    template <typename T>
    std::shared_ptr<T> RegisterGlobalSystem(Signature signature)
    {
        return system_manager_register_system<T>(Get().system_manager, signature);
    }

    template <typename T>
    void DeregisterSystem()
    {
        return system_manager_deregister_system<T>(Get().system_manager);
    }

    template <typename T>
    std::shared_ptr<T> GetSystemProvider()
    {
        return system_manager_get_systemprovider<T>(Get().system_manager);
    }
} // namespace Vultr

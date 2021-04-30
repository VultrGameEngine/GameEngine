#pragma once
#include "Game.hpp"
#include <ecs/component/component_registry.hpp>
#include <ecs/system/system_manager.hpp>
#include <GLFW/glfw3.h>

namespace Vultr
{
class Engine
{
  public:
    static Engine &Get();
    void Init(bool debug);
    void LoadGame(const std::string &path);
    void LoadGame(Game *game);
    static void RegisterComponents();
    void InitSystems();
    void InitGame();
    void UpdateGame(float &t);
    void Destroy();
    double GetElapsedTime();
    GLFWwindow *window;
    bool should_close = false;
    // Component methods
    template <typename T>
    static void RegisterComponent(bool inspector_available = true)
    {
        Get().registry->RegisterComponent<T>(inspector_available);
    }

    template <typename T>
    static std::shared_ptr<T> RegisterGlobalSystem(Signature signature)
    {
        return SystemManager::RegisterSystem<T>(Get().system_manager, signature);
    }

    template <typename T> static void DeregisterSystem()
    {
        return SystemManager::DeregisterSystem<T>(Get().system_manager);
    }

    template <typename T> static std::shared_ptr<T> GetSystemProvider()
    {
        return SystemManager::GetSystemProvider<T>();
    }

  private:
    Engine()
    {
        // registry = std::make_shared<ComponentRegistry>();
        // system_manager = std::make_shared<SystemManager::SystemManager>();
    }
    ~Engine()
    {
    }
    bool debug;
    Game *game;
    ComponentRegistry registry;
    SystemManager::SystemManager system_manager;
};
} // namespace Vultr

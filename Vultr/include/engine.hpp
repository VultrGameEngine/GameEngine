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
    static ComponentRegistry &GetComponentRegistry();
    static SystemManager &GetGlobalSystemManager();
    // Component methods
    template <typename T>
    static void RegisterComponent(bool inspector_available = true)
    {
        Get().registry->RegisterComponent<T>(inspector_available);
    }

    template <typename T>
    static std::shared_ptr<T> RegisterGlobalSystem(Signature signature)
    {
        return Get().system_manager->RegisterSystem<T>(signature);
    }
    template <typename T> static void SetSignature(Signature signature)
    {
        return Get().system_manager->SetSignature<T>(signature);
    }

    template <typename T> static void DeregisterSystem()
    {
        return Get().system_manager->DeregisterSystem<T>();
    }

    template <typename T> static std::shared_ptr<T> GetSystemProvider()
    {
        return Get().system_manager->GetSystemProvider<T>();
    }

  private:
    Engine()
    {
        registry = std::make_shared<ComponentRegistry>();
        system_manager = std::make_shared<SystemManager>();
    }
    ~Engine()
    {
    }
    bool debug;
    Game *game;
    std::shared_ptr<ComponentRegistry> registry;
    std::shared_ptr<SystemManager> system_manager;
};
} // namespace Vultr

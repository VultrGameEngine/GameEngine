#pragma once
#include <ecs/system/system_provider.hpp>
#include <memory.h>
#include <ecs/world/world.hpp>
#include <GLFW/glfw3.h>
#include <engine.hpp>

namespace Vultr
{
class ControllerSystemProvider : public SystemProvider
{
  public:
    // Singleton pattern for all providers
    static std::shared_ptr<ControllerSystemProvider> Get()
    {
        return Engine::GetSystemProvider<ControllerSystemProvider>();
    }
    GLFWwindow *m_window;
    bool m_focused = false;
    template <class Archive> void serialize(Archive &ar)
    {
        // We pass this cast to the base type for each base type we
        // need to serialize.  Do this instead of calling serialize functions
        // directly
        ar(cereal::base_class<SystemProvider>(this));
    }

  protected:
    void OnDestroyEntity(Entity entity) override;
    void OnCreateEntity(Entity entity) override;
};
} // namespace Vultr
VultrRegisterSystemProvider(Vultr::ControllerSystemProvider)

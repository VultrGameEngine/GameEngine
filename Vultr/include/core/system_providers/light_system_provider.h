#pragma once
#include <ecs/system/system_provider.hpp>
#include <engine.hpp>

namespace Vultr
{
class LightSystemProvider : public SystemProvider
{
  public:
    // Singleton pattern for all providers
    static std::shared_ptr<LightSystemProvider> Get()
    {
        return Engine::GetSystemProvider<LightSystemProvider>();
    }
    Entity light = Entity(-1);

  protected:
    void OnDestroyEntity(Entity entity) override;
    void OnCreateEntity(Entity entity) override;
};
} // namespace Vultr
VultrRegisterSystemProvider(Vultr::LightSystemProvider)

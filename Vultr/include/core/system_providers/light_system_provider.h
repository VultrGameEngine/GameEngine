#pragma once
#include <ecs/system/system_provider.hpp>
#include <ecs/world/world.hpp>
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

    template <class Archive> void serialize(Archive &archive)
    {
        archive(light); // Nothing needs to be serialized here
    }

  protected:
    void OnDestroyEntity(Entity entity) override;
    void OnCreateEntity(Entity entity) override;
};
} // namespace Vultr
VultrRegisterSystemProvider(Vultr::LightSystemProvider)

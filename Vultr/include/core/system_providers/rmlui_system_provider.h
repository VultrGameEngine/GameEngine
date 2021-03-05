#pragma once
#include <rmlui/rmlui.h>
#include <ecs/system/system_provider.hpp>
#include <ecs/world/world.hpp>

namespace Vultr
{
class RmlUiSystemProvider : public SystemProvider
{
  public:
    static std::shared_ptr<RmlUiSystemProvider> Get()
    {
        return World::GetSystemProvider<RmlUiSystemProvider>();
    }

    Rml::Context *context;

    // protected:
    //   void OnDestroyEntity(Entity entity) override;
    //   void OnCreateEntity(Entity entity) override;
};

} // namespace Vultr

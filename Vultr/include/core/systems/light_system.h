#pragma once
#include <core/system_providers/light_system_provider.h>
#include <ecs/system/system.hpp>
#include <glm/glm.hpp>

namespace Vultr
{
class LightSystem : public System
{
  public:
    static void RegisterSystem();

  protected:
    void OnCreateEntity(Entity entity) override;
    SystemProvider &GetProvider() override
    {
        return LightSystemProvider::Get();
    }
};
} // namespace Vultr

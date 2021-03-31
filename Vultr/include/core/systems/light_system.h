#pragma once
#include <core/system_providers/light_system_provider.h>
#include <glm/glm.hpp>

namespace Vultr
{
class LightSystem
{
  public:
    static void RegisterSystem();
    static void OnCreateEntity(Entity entity);

  protected:
    SystemProvider &GetProvider()
    {
        std::shared_ptr<LightSystemProvider> provider = LightSystemProvider::Get();
        return *provider;
    }
};
} // namespace Vultr

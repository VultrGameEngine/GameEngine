#pragma once
#include <ecs/system/system_provider.hpp>
#include <fonts/font.h>
#include <engine.hpp>
#include FT_FREETYPE_H

namespace Vultr
{
class FontSystemProvider : public SystemProvider
{
  public:
    static std::shared_ptr<FontSystemProvider> Get()
    {
        return Engine::GetGlobalSystemManager()
            .GetSystemProvider<FontSystemProvider>();
    }
    FT_Library library;

    std::unordered_map<std::string, Font *> fonts;

  protected:
    void OnCreateEntity(Entity entity) override;
    void OnDestroyEntity(Entity entity) override;
};
} // namespace Vultr
VultrRegisterSystemProvider(Vultr::FontSystemProvider)

#pragma once
#include <ecs/system/system_provider.hpp>
#include <ecs/world/world.hpp>
#include <fonts/font.h>
#include <engine.hpp>
#include <ft2build.h>
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
    template <class Archive> void serialize(Archive &ar)
    {
        // We pass this cast to the base type for each base type we
        // need to serialize.  Do this instead of calling serialize functions
        // directly
        ar(cereal::base_class<SystemProvider>(this));
    }

  protected:
    void OnCreateEntity(Entity entity) override;
    void OnDestroyEntity(Entity entity) override;
};
} // namespace Vultr
VultrRegisterSystemProvider(Vultr::FontSystemProvider)

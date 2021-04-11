#pragma once
#include <ecs/system/system_provider.hpp>
#include <rendering/models/texture.h>
#include <unordered_map>
#include <vector>
#include <ecs/world/world.hpp>
#include <engine.hpp>

namespace Vultr
{
class TextureLoaderSystemProvider : public SystemProvider
{
  public:
    static std::shared_ptr<TextureLoaderSystemProvider> Get()
    {
        return Engine::GetSystemProvider<TextureLoaderSystemProvider>();
    }
    static Texture *GetTexture(const std::string &texture);
    static bool isLoaded(const std::string &texture);

    std::unordered_map<std::string, Texture *> textures;
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
VultrRegisterSystemProvider(Vultr::TextureLoaderSystemProvider)

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

    template <class Archive> void serialize(Archive &archive)
    {
        archive(); // Nothing needs to be serialized here
    }

  protected:
    void OnCreateEntity(Entity entity) override;
    void OnDestroyEntity(Entity entity) override;
};
} // namespace Vultr
VultrRegisterSystemProvider(Vultr::TextureLoaderSystemProvider)

#pragma once
#include <ecs/system/system_provider.hpp>
#include <rendering/models/texture.h>
#include <unordered_map>
#include <vector>
#include <ecs/world/world.hpp>

namespace Vultr
{
class TextureLoaderSystemProvider : public SystemProvider
{
  public:
    static std::shared_ptr<TextureLoaderSystemProvider> Get()
    {
        return World::GetSystemProvider<TextureLoaderSystemProvider>();
    }
    Texture *GetTexture(const std::string &texture);
    bool isLoaded(const std::string &texture);

    std::unordered_map<std::string, Texture *> textures;

  protected:
    void OnCreateEntity(Entity entity) override;
    void OnDestroyEntity(Entity entity) override;
};
} // namespace Vultr

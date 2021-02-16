#pragma once
#include <ecs/system/system_provider.hpp>
#include <rendering/models/texture.h>
#include <unordered_map>
#include <vector>

namespace Vultr
{
class TextureLoaderSystemProvider : public SystemProvider
{
  public:
    static TextureLoaderSystemProvider &Get()
    {
        static TextureLoaderSystemProvider instance;
        return instance;
    }
    Texture *GetTexture(const std::string &texture);
    bool isLoaded(const std::string &texture);
    void Reset() override
    {
        for (auto [id, shader] : textures)
        {
            delete shader;
        }
        textures.clear();
    }

    std::unordered_map<std::string, Texture *> textures;
};
} // namespace Vultr

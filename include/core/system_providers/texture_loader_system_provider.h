#pragma once
#include <ecs/system/system_provider.hpp>
#include <rendering/models/texture.h>
#include <unordered_map>
#include <vector>

namespace Brick3D
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

    std::unordered_map<std::string, Texture *> textures;
};
} // namespace Brick3D

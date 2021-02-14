#include <core/system_providers/texture_loader_system_provider.h>

namespace Brick3D
{
Texture *TextureLoaderSystemProvider::GetTexture(const std::string &texture)
{
    if (isLoaded(texture))
    {
        return textures[texture];
    }
    else
    {
        return nullptr;
    }
}

bool TextureLoaderSystemProvider::isLoaded(const std::string &texture)
{
    return textures.find(texture) != textures.end();
}
} // namespace Brick3D

#include <core/system_providers/texture_loader_system_provider.h>

namespace Vultr::TextureLoaderSystem
{
    Component &get_provider()
    {
        return *get_global_system_provider<Component>();
    }
    Texture *get_texture(const TextureSource &texture)
    {
        return get_texture(texture.path.string().c_str());
    }

    Texture *get_texture(const char *texture)
    {
        if (is_loaded(texture))
        {
            return get_provider().textures[texture];
        }
        else
        {
            return nullptr;
        }
    }

    bool is_loaded(const TextureSource &texture)
    {
        return is_loaded(texture.path.string().c_str());
    }

    bool is_loaded(const char *texture)
    {
        auto &p = get_provider();
        return p.textures.find(texture) != p.textures.end();
    }
} // namespace Vultr::TextureLoaderSystem

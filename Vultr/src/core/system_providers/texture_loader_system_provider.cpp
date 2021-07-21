#include <core/system_providers/texture_loader_system_provider.h>

namespace Vultr::TextureLoaderSystem
{
    Component &get_provider(Engine *e)
    {
        return *get_global_system_provider<Component>(e);
    }
    Texture *get_texture(Engine *e, const TextureSource &texture)
    {
        return get_texture(e, texture.path.string().c_str());
    }

    Texture *get_texture(Engine *e, const char *texture)
    {
        if (is_loaded(e, texture))
        {
            return &get_provider(e).textures[texture];
        }
        else
        {
            return nullptr;
        }
    }

    bool is_loaded(Engine *e, const TextureSource &texture)
    {
        return is_loaded(e, texture.path.string().c_str());
    }

    bool is_loaded(Engine *e, const char *texture)
    {
        auto &p = get_provider(e);
        return p.textures.find(texture) != p.textures.end();
    }
} // namespace Vultr::TextureLoaderSystem

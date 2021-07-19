#pragma once
#include <ecs/system/system_provider.hpp>
#include <rendering/types/texture.h>
#include <unordered_map>
#include <engine.hpp>
#include <type_info/type_info.h>

namespace Vultr
{
    namespace TextureLoaderSystem
    {
        struct Component : public SystemProvider
        {
            std::unordered_map<std::string, Texture> textures;
        };
        Component &get_provider();
        Texture *get_texture(const TextureSource &texture);
        Texture *get_texture(const char *texture);
        bool is_loaded(const TextureSource &texture);
        bool is_loaded(const char *texture);
    } // namespace TextureLoaderSystem

    template <>
    inline const char *get_struct_name<TextureLoaderSystem::Component>()
    {
        return "TextureLoaderSystem";
    }
} // namespace Vultr

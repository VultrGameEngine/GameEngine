#pragma once
#include <ecs/system/system_provider.hpp>
#include <fonts/font.h>
#include <engine.hpp>
#include FT_FREETYPE_H

namespace Vultr
{
    namespace FontSystem
    {
        struct Component : public SystemProvider
        {
            FT_Library library;
            std::unordered_map<const char *, Font *> fonts;
        };

        Component &get_provider(Engine *e);

    } // namespace FontSystem

    template <>
    inline const char *get_struct_name<FontSystem::Component>()
    {
        return "FontSystem";
    }
} // namespace Vultr

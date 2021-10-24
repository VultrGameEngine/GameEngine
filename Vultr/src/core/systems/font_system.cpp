#include <core/systems/font_system.h>
#include <core/system_providers/font_system_provider.h>
#include <filesystem/importers/font_importer.h>
#include <engine.hpp>

namespace Vultr::FontSystem
{
    void register_system(Engine *e)
    {
        Signature signature;
        register_global_system<Component>(e, signature, nullptr, nullptr);
    }

    void init(Engine *e)
    {
        auto &provider = get_provider(e);
        if (FT_Init_FreeType(&provider.library))
        {
            assert("Could not init the freetype library");
        }
    }

    // TODO: More ugly shit
    void preload_font(Engine *e, const FontSource &source)
    {
        auto &provider = get_provider(e);
        provider.fonts[std::string(source.path).c_str()] = FontImporter::import_font(source, provider.library);
    }
} // namespace Vultr::FontSystem

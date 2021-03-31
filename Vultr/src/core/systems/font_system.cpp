#include <core/systems/font_system.h>
#include <helpers/font_importer.h>

namespace Vultr
{
void FontSystem::RegisterSystem()
{
    Signature signature;
    World::RegisterSystem<FontSystemProvider>(signature);
}

void FontSystem::Init()
{
    FontSystemProvider &provider = GetProvider();
    if (FT_Init_FreeType(&provider.library))
    {
        assert("Could not init the freetype library");
    }
}
void FontSystem::PreloadFont(const std::string &path)
{
    FontSystemProvider &provider = GetProvider();
    provider.fonts[path] = FontImporter::ImportFont(path, provider.library);
}
} // namespace Vultr

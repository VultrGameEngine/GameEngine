#pragma once
#include <fonts/font.h>
#include <helpers/file.h>
#include <string>
#define TEXT_SCALE_FACTOR 24

namespace Vultr::FontImporter
{
    Font *import_font(const FontSource &source, const FT_Library &library);

} // namespace Vultr::FontImporter

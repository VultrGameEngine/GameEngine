#pragma once
#include <fonts/font.h>
#include <string>
#define TEXT_SCALE_FACTOR 48000

namespace Vultr
{
class FontImporter
{
  public:
    static Font *ImportFont(const std::string &path, const FT_Library &library);
};

} // namespace Vultr

#pragma once
#include <fonts/font.h>
#include <string>

namespace Vultr
{
class FontImporter
{
  public:
    static Font *ImportFont(const std::string &path, const FT_Library &library,
                            double height);
};

} // namespace Vultr

#pragma once
#include <rendering/models/texture.h>
#include <string>
#include <vector>

namespace Vultr
{
class TextureImporter
{
  public:
    static bool Import(const std::string &path, Texture &texture);
    static bool ImportSkybox(const std::vector<std::string> &paths,
                             Texture &texture);
};
} // namespace Vultr

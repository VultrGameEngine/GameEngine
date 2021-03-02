#pragma once
#include <rendering/models/texture.h>
#include <string>
#include <vector>

namespace Vultr
{
class TextureImporter
{
  public:
    static void Import(const std::string &path, Texture &texture);
    static void ImportSkybox(const std::vector<std::string> &paths,
                             Texture &texture);
};
} // namespace Vultr

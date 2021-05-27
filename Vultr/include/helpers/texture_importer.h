#pragma once
#include <rendering/models/texture.h>
#include <string>
#include <types/types.hpp>

namespace Vultr
{
    class TextureImporter
    {
      public:
        static bool Import(const std::string &path, Texture &texture);
        static void Import(const unsigned char *data, uint size, Texture &texture);
        static void Import(const unsigned char *data, Texture &texture, u32 width, u32 height);
        static bool ImportSkybox(const std::vector<std::string> &paths, Texture &texture);
    };
} // namespace Vultr

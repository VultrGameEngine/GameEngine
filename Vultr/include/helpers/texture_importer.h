#pragma once
#include <rendering/models/texture.h>
#include <string>
#include <types/types.hpp>
#include <helpers/file.h>

namespace Vultr::TextureImporter
{
    bool import(const TextureSource &source, Texture &texture);
    void import(const unsigned char *data, u32 size, Texture &texture);
    void import(const unsigned char *data, Texture &texture, u32 width, u32 height);
    bool import_skybox(const std::vector<TextureSource> &paths, Texture &texture);
} // namespace Vultr::TextureImporter

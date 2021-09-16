#pragma once
#include <rendering/types/texture.h>
#include <string>
#include <types/types.hpp>
#include <filesystem/file.h>

namespace Vultr::TextureImporter
{
    bool import(Texture *texture, const TextureSource &source);
    void import(Texture *texture, const unsigned char *data, u32 size);
    void import(Texture *texture, const unsigned char *data, u32 width, u32 height);
    bool import_skybox(Texture *texture, const std::vector<TextureSource> &paths);
} // namespace Vultr::TextureImporter

#pragma once
#include <types/types.hpp>

namespace Vultr
{
    struct InternalTexture
    {
        u32 id = 0;
        u32 width = 0;
        u32 height = 0;
        u32 level = 0;
        u32 type = GL_TEXTURE_2D;
        u32 internal_format = GL_RGB;
        u32 format = GL_RGB;
        u32 pixel_data_type = GL_UNSIGNED_BYTE;

        InternalTexture() = default;
        InternalTexture(u32 type);
    };

    void generate_texture(InternalTexture *texture, GLenum type);

#define invalid_texture()                                                                                                                                                                                             \
    InternalTexture                                                                                                                                                                                                   \
    {                                                                                                                                                                                                                 \
    }
} // namespace Vultr

#pragma once
#include <types/types.hpp>

namespace Vultr
{
    struct InternalTexture;
    typedef InternalTexture Texture;

    void texture_image_2D(Texture *texture, u32 level, GLenum internalformat, u32 width, u32 height, GLenum format, GLenum type, const void *data);
    void texture_image_2D(GLenum target, u32 level, GLenum internalformat, u32 width, u32 height, GLenum format, GLenum type, const void *data);
    void texture_parameter_i(Texture *texture, GLenum pname, u32 param);

    bool is_valid_texture(Texture *texture);
    void delete_texture(Texture *texture);

    void bind_texture(Texture *texture, GLenum slot);
    void unbind_texture(Texture *texture);
} // namespace Vultr

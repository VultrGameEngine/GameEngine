#include <rendering/types/texture.h>
#include <glad/glad.h>

namespace Vultr
{
    Texture generate_texture(u32 type)
    {
        Texture texture = {.type = type};
        glGenTextures(1, &texture.id);
        return texture;
    }

    Texture invalid_texture()
    {
        return {.id = 0};
    }

    void texture_image_2D(Texture &texture, u32 level, GLenum internalformat, u32 width, u32 height, GLenum format, GLenum type, const void *data)
    {
        bind_texture(texture, GL_TEXTURE0);
        glTexImage2D(texture.type, level, internalformat, width, height, 0, format, type, data);
    }

    void texture_image_2D(GLenum target, u32 level, GLenum internalformat, u32 width, u32 height, GLenum format, GLenum type, const void *data)
    {
        glTexImage2D(target, level, internalformat, width, height, 0, format, type, data);
    }

    void texture_parameter_i(Texture &texture, GLenum pname, u32 param)
    {
        bind_texture(texture, GL_TEXTURE0);
        glTextureParameteri(texture.id, pname, param);
    }

    bool is_valid_texture(Texture &texture)
    {
        return texture.id != 0;
    }

    void delete_texture(Texture &texture)
    {
        glDeleteTextures(1, &texture.id);
    }

    void bind_texture(Texture &texture, u32 slot)
    {
        texture.bound = true;
        glActiveTexture(slot);
        glBindTexture(texture.type, texture.id);
    }

    void unbind_texture(Texture &texture)
    {
        texture.bound = false;
        glBindTexture(texture.type, 0);
    }
} // namespace Vultr

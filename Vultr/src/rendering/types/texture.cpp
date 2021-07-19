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

// namespace Vultr
// {
// Texture::Texture(GLenum type) : m_height(0), m_width(0)
// {
//     this->m_type = type;
//     glGenTextures(1, &m_id);
// }
// Texture::Texture(GLenum type, unsigned int p_id) : m_height(0), m_width(0)
// {
//     this->m_type = type;
//     this->m_id = p_id;
// }
// Texture::~Texture()
// {
//     glDeleteTextures(1, &m_id);
// }

// void Texture::Bind(GLenum slot)
// {
//     glActiveTexture(slot);
//     glBindTexture(this->m_type, this->m_id);
// }

// void Texture::Unbind(GLenum slot)
// {
//     glActiveTexture(slot);
//     glBindTexture(this->m_type, 0);
// }

// void Texture::Generate(unsigned int width, unsigned int height)
// {
//     this->m_width = width;
//     this->m_height = height;
//     Bind(GL_TEXTURE0);
//     glTexImage2D(m_type, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
// }

// void Texture::FrameBufferTexture2D()
// {
//     Bind(GL_TEXTURE0);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//     glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_id, 0);
// }
// } // namespace Vultr

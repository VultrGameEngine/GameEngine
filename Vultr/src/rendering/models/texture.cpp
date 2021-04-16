#include <rendering/models/texture.h>

namespace Vultr
{
Texture::Texture(GLenum type) : m_height(0), m_width(0)
{
    this->m_type = type;
    glGenTextures(1, &m_id);
}
Texture::Texture(GLenum type, unsigned int p_id) : m_height(0), m_width(0)
{
    this->m_type = type;
    this->m_id = p_id;
}
Texture::~Texture()
{
    glDeleteTextures(1, &m_id);
}

void Texture::Bind(GLenum slot)
{
    glActiveTexture(slot);
    glBindTexture(this->m_type, this->m_id);
}

void Texture::Unbind(GLenum slot)
{
    glActiveTexture(slot);
    glBindTexture(this->m_type, 0);
}

void Texture::Generate(unsigned int width, unsigned int height)
{
    this->m_width = width;
    this->m_height = height;
    Bind(GL_TEXTURE0);
    glTexImage2D(m_type, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 nullptr);
}

void Texture::FrameBufferTexture2D()
{
    Bind(GL_TEXTURE0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_id,
                           0);
}
} // namespace Vultr

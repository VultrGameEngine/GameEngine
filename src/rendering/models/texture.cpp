#include <rendering/models/texture.h>

namespace Brick3D
{
Texture::Texture(GLenum type)
{
    this->m_type = type;
    glCreateTextures(type, 1, &m_id);
}
Texture::~Texture()
{
    glDeleteTextures(1, &m_id);
}

void Texture::Bind()
{
    glBindTexture(this->m_type, this->m_id);
}

void Texture::Unbind()
{
    glBindTexture(this->m_type, 0);
}

void Texture::Generate(unsigned int width, unsigned int height)
{
    this->m_width = width;
    this->m_height = height;
    Bind();
    glTexImage2D(m_type, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 nullptr);
}

void Texture::FrameBufferTexture2D()
{
    Bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_id,
                           0);
}
} // namespace Brick3D

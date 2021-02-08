#include <rendering/models/texture.h>

namespace Brick3D
{
Texture::Texture(GLenum type) : m_type(type)
{
    glCreateTextures(m_type, 1, &this->m_id);
}
Texture::~Texture()
{
    glDeleteTextures(1, &this->m_id);
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
    Bind();
    glTexImage2D(m_type, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 nullptr);
}
} // namespace Brick3D

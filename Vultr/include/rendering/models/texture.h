#pragma once
#include <glad/glad.h>

namespace Vultr
{
class Texture
{
  public:
    Texture(GLenum type);
    Texture(GLenum type, unsigned int p_id);
    ~Texture();

    void Generate(unsigned int width, unsigned int height);
    void Bind(GLenum slot);
    void Unbind(GLenum slot);

    unsigned int GetID() const
    {
        return this->m_id;
    }

    void FrameBufferTexture2D();

  private:
    unsigned int m_id;
    GLenum m_type;
    unsigned int m_width;
    unsigned int m_height;
};
} // namespace Vultr

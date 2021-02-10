#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Brick3D
{
class Texture
{
  public:
    Texture(GLenum type);
    ~Texture();

    void Generate(unsigned int width, unsigned int height);
    void Bind();
    void Unbind();

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
} // namespace Brick3D

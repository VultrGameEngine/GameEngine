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

  private:
    unsigned int m_id;
    GLenum m_type;
};
} // namespace Brick3D

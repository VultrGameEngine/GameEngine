#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Brick3D
{
class IndexBuffer
{
  public:
    IndexBuffer(const GLvoid *indices, unsigned int count);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

  private:
    unsigned int id;
};
} // namespace Brick3D

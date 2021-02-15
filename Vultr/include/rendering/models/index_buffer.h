#pragma once
#include <glad/glad.h>

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

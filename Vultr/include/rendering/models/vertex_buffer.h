#pragma once
#include "vertex.h"
#include <glad/glad.h>
#include <vector>

namespace Brick3D
{
class VertexBuffer
{
  public:
    VertexBuffer(const GLvoid *vertices, unsigned int count);
    ~VertexBuffer();

    void Bind();
    void Unbind();

  private:
    unsigned int id;
};
} // namespace Brick3D

#pragma once
#include "vertex.h"
#include <glad/glad.h>
#include <vector>

namespace Vultr
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
} // namespace Vultr

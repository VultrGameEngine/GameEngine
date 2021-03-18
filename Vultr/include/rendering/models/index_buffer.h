#pragma once
#include <glad/glad.h>

namespace Vultr
{
class IndexBuffer
{
  public:
    IndexBuffer(const GLvoid *indices, unsigned int count);
    IndexBuffer();

    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

  private:
    unsigned int id;
};
} // namespace Vultr

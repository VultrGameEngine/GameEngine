#pragma once
#include <glad/glad.h>
#include <assert.h>
#include <glm/glm.hpp>
#include <rendering/models/vertex.h>

namespace Vultr
{

class VertexArray
{
  public:
    VertexArray();
    ~VertexArray();

    template <typename T> void Setup()
    {
        assert(false && "Unsupported buffer layout type");
    }

    void Bind() const;
    void Unbind() const;

  private:
    unsigned int id;
};

} // namespace Vultr

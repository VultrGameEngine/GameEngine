#pragma once
#include <GL/glew.h>
#include <assert.h>
#include <glm/glm.hpp>
#include <rendering/models/vertex.h>
#include <vector>

namespace Brick3D
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

    template <> void Setup<Vertex>()
    {
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (const void *)offsetof(Vertex, position));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (const void *)offsetof(Vertex, normal));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (const void *)offsetof(Vertex, uv));
    }

    void Bind();
    void Unbind();

  private:
    unsigned int id;
};

} // namespace Brick3D

#include <rendering/models/vertex.h>
#include <rendering/models/vertex_buffer.h>

namespace Brick3D
{

VertexBuffer::VertexBuffer(const GLvoid *vertices, unsigned int count)
{
    // Create and set the vertex buffer data
    glCreateBuffers(1, &this->id);
    Bind();
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * count, vertices, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &this->id);
}

void VertexBuffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, this->id);
}
void VertexBuffer::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

} // namespace Brick3D

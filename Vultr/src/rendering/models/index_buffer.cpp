#include <rendering/models/index_buffer.h>

namespace Vultr
{
IndexBuffer::IndexBuffer(const GLvoid *indices, unsigned int count)
{
    glCreateBuffers(1, &this->id);
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * count, indices,
                 GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &this->id);
}

void IndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
}

void IndexBuffer::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

} // namespace Vultr

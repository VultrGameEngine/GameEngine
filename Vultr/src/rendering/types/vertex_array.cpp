#include <rendering/types/vertex_array.h>

namespace Vultr
{
VertexArray::VertexArray()
{
    glGenVertexArrays(1, &this->id);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &this->id);
}

void VertexArray::Bind() const
{
    glBindVertexArray(this->id);
}

void VertexArray::Unbind() const
{
    glBindVertexArray(0);
}
} // namespace Vultr

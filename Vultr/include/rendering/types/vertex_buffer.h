#pragma once
#include "vertex.h"
#include <types/types.hpp>

namespace Vultr
{
    typedef u32 VertexBuffer;

    VertexBuffer new_vertex_buffer(u32 size);
    VertexBuffer new_vertex_buffer(const void *vertices, size_t count);

    void delete_vertex_buffer(VertexBuffer vbo);

    void bind_vertex_buffer(VertexBuffer vbo);
    void unbind_vertex_buffer();
} // namespace Vultr

#pragma once
#include <rendering/models/index_buffer.h>
#include <rendering/models/vertex_buffer.h>
#include <rendering/models/vertex_array.h>
#include <gui/rendering/gui_vertex.h>
#include "quad.h"

namespace Vultr
{
    namespace IMGUI
    {
#define MAX_QUADS 1000
        struct QuadBatch
        {
            IndexBuffer *ibo;
            VertexBuffer *vbo;
            VertexArray *vao;
            unsigned short *indices;
            IMGUI::GUIVertex *vertices;
        };

        QuadBatch *new_quad_batch();
        void destroy_quad_batch(QuadBatch *batch);

        void quad_batch_push_quads(QuadBatch *b, Quad *quads, u32 num_quads);

        void flush_quad_batch(QuadBatch *b);

        void quad_batch_draw(QuadBatch *b, u32 num_quads);
    } // namespace IMGUI
} // namespace Vultr

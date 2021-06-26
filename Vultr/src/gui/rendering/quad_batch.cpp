#include <gui/rendering/quad_batch.h>

using namespace Vultr;

IMGUI::QuadBatch *IMGUI::new_quad_batch()
{
    auto *b = new QuadBatch();
    b->vao = new VertexArray();
    b->vao->Bind();
    b->vbo = new VertexBuffer(sizeof(IMGUI::GUIVertex) * MAX_QUADS * 4);
    b->vbo->Bind();
    b->vao->Setup<IMGUI::GUIVertex>();
    u32 offset = 0;
    b->indices = new unsigned short[MAX_QUADS * 6];
    for (int i = 0; i < MAX_QUADS * 6; i += 6)
    {
        b->indices[i + 0] = 1 + offset;
        b->indices[i + 1] = 2 + offset;
        b->indices[i + 2] = 3 + offset;

        b->indices[i + 3] = 1 + offset;
        b->indices[i + 4] = 0 + offset;
        b->indices[i + 5] = 2 + offset;
        offset += 4;
    }
    b->ibo = new IndexBuffer(b->indices, MAX_QUADS * 6);

    b->vertices = new IMGUI::GUIVertex[MAX_QUADS * 4];

    return b;
}

void IMGUI::quad_batch_push_quads(QuadBatch *b, Quad *quads, u32 num_quads)
{
    b->vertices = static_cast<GUIVertex *>(malloc(sizeof(GUIVertex) * num_quads * 4));
    s32 quad_index = 0;
    for (s32 i = 0; i < num_quads * 4; i += 4)
    {
        auto &quad = quads[quad_index];
        b->vertices[i + 0] = quad.vertices[0];
        b->vertices[i + 1] = quad.vertices[1];
        b->vertices[i + 2] = quad.vertices[2];
        b->vertices[i + 3] = quad.vertices[3];
        quad_index++;
    }
    b->vbo->Bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, num_quads * 4 * sizeof(GUIVertex), b->vertices);
    flush_quad_batch(b);
}

void IMGUI::flush_quad_batch(QuadBatch *b)
{
    if (b->vertices == nullptr)
        return;
    free(b->vertices);
    b->vertices = nullptr;
}

void IMGUI::quad_batch_draw(QuadBatch *b, u32 num_quads)
{
    b->vbo->Bind();
    b->vao->Bind();
    b->ibo->Bind();
    glDrawElements(GL_TRIANGLES, num_quads * 6, GL_UNSIGNED_SHORT, (void *)0);
}

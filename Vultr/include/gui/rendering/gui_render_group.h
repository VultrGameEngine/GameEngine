#pragma once
#include <rendering/models/vertex_array.h>
#include <rendering/models/vertex_buffer.h>
#include <rendering/models/index_buffer.h>
#include "gui_vertex.h"
#include "quad.h"
#include <array>
#include <unordered_map>
#include <queue>
#define MAX_QUADS 1000

namespace Vultr
{
namespace GUI
{

class RenderGroup
{
  public:
    RenderGroup()
    {
        vao = new VertexArray();
        vao->Bind();
        vbo = new VertexBuffer(sizeof(GUI::GUIVertex) * MAX_QUADS * 4);
        vbo->Bind();
        vao->Setup<GUI::GUIVertex>();
        int offset = 0;
        num_quads = 0;
        for (int i = 0; i < MAX_QUADS * 6; i += 6)
        {
            indices[i + 0] = 0 + offset;
            indices[i + 1] = 1 + offset;
            indices[i + 2] = 2 + offset;

            indices[i + 3] = 2 + offset;
            indices[i + 4] = 3 + offset;
            indices[i + 5] = 0 + offset;
            offset += 4;
        }
        ibo = new IndexBuffer(indices, MAX_QUADS * 6);
    }

    unsigned int GetWidgetCount()
    {
        return num_quads;
    }

    Quad GetQuadAtIndex(int index)
    {
        assert(index <= (num_quads - 1) * 4 && "Index out of bounds");
        return Quad(vertices[index], vertices[index + 1], vertices[index + 2],
                    vertices[index + 3]);
    }
    bool DeleteQuadAtIndex(int index)
    {
        assert(index <= (num_quads - 1) * 4 && "Index out of bounds");
        if (num_quads > 1)
        {
            for (int i = 0; i < 4; i++)
            {
                vertices[index + i] = vertices[(num_quads - 1) * 4 + i];
            }
        }
        num_quads--;
        return true;
    }

    int SubmitQuad()
    {
        assert(num_quads < MAX_QUADS && "Reached max quads in this layer");
        int index = num_quads * 4;
        num_quads++;
        return index;
    }

    void Draw()
    {
        vbo->Bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        vao->Bind();
        ibo->Bind();
        glDrawElements(GL_TRIANGLES, num_quads * 6, GL_UNSIGNED_SHORT, (void *)0);
    }

  private:
    VertexArray *vao;
    VertexBuffer *vbo;
    IndexBuffer *ibo;

    unsigned short indices[MAX_QUADS * 6];
    GUI::GUIVertex vertices[MAX_QUADS * 4];

    unsigned int num_quads = 0;
    // std::queue<WidgetID> available_widgets;

    // Quad quads[MAX_QUADS];

    // std::unordered_map<uint32_t, WidgetID> vertex_to_quad;
};
} // namespace GUI
} // namespace Vultr

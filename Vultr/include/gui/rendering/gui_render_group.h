#pragma once
#include <rendering/models/vertex_array.h>
#include <rendering/models/vertex_buffer.h>
#include <rendering/models/index_buffer.h>
#include <rendering/models/texture.h>
#include "gui_vertex.h"
#include <string>
#include "quad.h"
#include <array>
#include <unordered_map>
#include <queue>
#define MAX_QUADS 1000
#define MAX_TEXTURES 16

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
        // If there is no room for either quads or textures, we need to tell whoever
        // submitted to us that we don't have room
        if (num_quads >= MAX_QUADS)
        {
            return -1;
        }
        else if (textures.size() >= MAX_TEXTURES)
        {
            return -1;
        }
        int index = num_quads * 4;
        num_quads++;
        return index;
    }

    void Draw()
    {
        // Draw all of our dependencies (basically just framebuffers that have to get
        // rendered first)
        for (RenderGroup *group : dependent_render_groups)
        {
            group->Draw();
        }

        // Then bind all of our textures
        int counter = 0;
        for (auto [id, texture] : textures)
        {
            texture->Bind(GL_TEXTURE0 + counter);
            counter++;
        }

        // Then render our quad
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

    std::vector<RenderGroup *> dependent_render_groups;

    std::unordered_map<std::string, Texture *> textures;
    // std::queue<WidgetID> available_widgets;

    // Quad quads[MAX_QUADS];

    // std::unordered_map<uint32_t, WidgetID> vertex_to_quad;
};
} // namespace GUI
} // namespace Vultr

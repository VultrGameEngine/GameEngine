#pragma once
#include <rendering/models/vertex_array.h>
#include <rendering/models/vertex_buffer.h>
#include <rendering/models/index_buffer.h>
#include <rendering/models/texture.h>
#include <rendering/models/shader.h>
#include <helpers/texture_importer.h>
#include <gui/rendering/quad.h>
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
        for (QuadID quad = 0; quad < MAX_QUADS; quad++)
        {
            available_quad_ids.push(quad);
        }

        for (int i = 0; i < MAX_TEXTURES; i++)
        {
            textures[i] = nullptr;
            samplers[i] = i;
        }

        ibo = new IndexBuffer(indices, MAX_QUADS * 6);
    }

    unsigned int GetQuadCount()
    {
        return num_quads;
    }

    Quad GetQuad(QuadID id)
    {
        assert(quad_to_vertex.find(id) != quad_to_vertex.end() &&
               "Quad does not exist");
        VertexID index = quad_to_vertex[id];
        int texture_slot = -1;
        if (quad_to_texture.find(id) != quad_to_texture.end())
        {
            texture_slot = quad_to_texture[id];
        }
        return Quad(vertices[index], vertices[index + 1], vertices[index + 2],
                    vertices[index + 3], texture_slot);
    }

    bool DeleteQuad(QuadID id)
    {
        assert(quad_to_vertex.find(id) != quad_to_vertex.end() &&
               "Quad does not exist");
        VertexID index = quad_to_vertex[id];
        quad_to_vertex.erase(id);
        if (num_quads > 1)
        {
            // Get the ids of the last vertex and last quad
            VertexID last_vertex = (num_quads - 1) * 4;
            QuadID last_quad = vertex_to_quad[last_vertex];

            // Remove the last vertex
            vertex_to_quad.erase(last_vertex);

            // Change the last quad to direct to the hole (index)
            quad_to_vertex[last_quad] = index;
            for (int i = 0; i < 4; i++)
            {
                // Move all of the vertices to fill the hole
                vertices[index + i] = vertices[last_vertex + i];
            }
        }
        available_quad_ids.push(id);
        num_quads--;
        return true;
    }

    QuadID SubmitQuad(Texture *texture)
    {
        QuadID quad = available_quad_ids.front();
        // If there is no room for either quads or textures, we need to tell whoever
        // submitted to us that we don't have room
        if (num_quads >= MAX_QUADS)
        {
            return -1;
        }
        else if (texture != nullptr)
        {
            bool already_have_texture = false;
            for (int i = 0; i < num_textures; i++)
            {
                if (textures[i] == texture)
                {
                    already_have_texture = true;
                    quad_to_texture[quad] = i;
                }
            }
            if (!already_have_texture)
            {
                if (num_textures < MAX_TEXTURES)
                {
                    textures[num_textures] = texture;
                    quad_to_texture[quad] = num_textures;
                    num_textures++;
                }
                else
                {
                    return -1;
                }
            }
        }
        available_quad_ids.pop();
        VertexID index = num_quads * 4;
        vertex_to_quad[index] = quad;
        quad_to_vertex[quad] = index;
        num_quads++;
        return quad;
    }

    void Draw(Shader *shader)
    {
        // Draw all of our dependencies (basically just framebuffers that have to get
        // rendered first)
        for (RenderGroup *group : dependent_render_groups)
        {
            group->Draw(shader);
        }

        // Then bind all of our textures
        int counter = 0;
        for (int i = 0; i < num_textures; i++)
        {
            textures[i]->Bind(GL_TEXTURE0 + counter);
            counter++;
        }

        glUniform1iv(shader->GetUniformLocation("u_Textures"), MAX_TEXTURES,
                     samplers);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

    unsigned int num_textures = 0;
    Texture *textures[MAX_TEXTURES];
    int samplers[MAX_TEXTURES];
    std::unordered_map<QuadID, VertexID> quad_to_vertex;
    std::unordered_map<QuadID, unsigned int> quad_to_texture;
    std::unordered_map<VertexID, QuadID> vertex_to_quad;
    std::queue<QuadID> available_quad_ids;
    // std::queue<WidgetID> available_widgets;

    // Quad quads[MAX_QUADS];

    // std::unordered_map<uint32_t, WidgetID> vertex_to_quad;
};
} // namespace GUI
} // namespace Vultr

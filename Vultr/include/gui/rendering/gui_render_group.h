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
#include <iostream>
#include <map>
#define MAX_QUADS 1000
#define MAX_TEXTURES 16

namespace Vultr
{
namespace GUI
{

class RenderGroup
{
  private:
    struct ZIndexGroup 
    {
        std::vector<VertexID> vertices;
        void AddVertex(VertexID vertex) 
        {
            vertices.push_back(vertex);
        }

        void RemoveVertex(VertexID vertex) 
        {
            for (int i = 0; i < vertices.size(); i++) 
            {
                if (vertices[i] == vertex) 
                {
                    vertices.erase(vertices.begin() + i);
                    return;
                }
            }
        }
    };
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

        for (VertexID vertex = 0; vertex < MAX_QUADS * 4; vertex += 4)
        {
            available_vertex_ids.push(vertex);
        }

        for (int i = 0; i < MAX_TEXTURES; i++)
        {
            textures[i] = nullptr;
            samplers[i] = i;
        }

        ibo = new IndexBuffer(sizeof(unsigned short) * MAX_QUADS * 6);
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
        z_index_groups[quad_to_zindex[id]]->RemoveVertex(quad_to_vertex[id]);
        quad_to_zindex.erase(id);
        quad_to_vertex.erase(id);
        vertex_to_quad.erase(index);
        available_quad_ids.push(id);
        available_vertex_ids.push(index);
        num_quads--;
        return true;
    }

    QuadID SubmitQuad(Zindex z_index, Texture *texture)
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
        VertexID index = available_vertex_ids.front();
        available_vertex_ids.pop();
        vertex_to_quad[index] = quad;
        quad_to_vertex[quad] = index;
        quad_to_zindex[quad] = -z_index;
        AddVertex(index, z_index);
        num_quads++;
        return quad;
    }
    
    void AddVertex(VertexID id, Zindex z_index) 
    {
        z_index = -z_index;
        if (z_index_groups.find(z_index) == z_index_groups.end()) 
        {
            z_index_groups[z_index] = new ZIndexGroup();
        }
        z_index_groups[z_index]->AddVertex(id);
    }

    void OrderIndices() 
    {
        IndexID index = 0;
        for (auto [z_index, group] : z_index_groups) 
        {
            for (VertexID vertex : group->vertices) 
            {
                indices[index + 0] = 0 + vertex;
                indices[index + 1] = 1 + vertex;
                indices[index + 2] = 2 + vertex;

                indices[index + 3] = 2 + vertex;
                indices[index + 4] = 3 + vertex;
                indices[index + 5] = 0 + vertex;
                index += 6;
            }

        }
    }

    void CommitQuad(QuadID id, Zindex z_index) 
    {
        z_index = -z_index;
        if (quad_to_zindex[id] == z_index) return;

        z_index_groups[quad_to_zindex[id]]->RemoveVertex(quad_to_vertex[id]);
        quad_to_zindex[id] = z_index;
        AddVertex(quad_to_vertex[id], z_index);
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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Then render our quad
        vbo->Bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        vao->Bind();
        ibo->Bind();
        OrderIndices();
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indices), indices);
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
    std::unordered_map<QuadID, Zindex> quad_to_zindex;
    std::map<Zindex, ZIndexGroup*> z_index_groups;
    std::queue<QuadID> available_quad_ids;
    std::queue<VertexID> available_vertex_ids;
    
    bool needs_reorder = true;
};
} // namespace GUI
} // namespace Vultr

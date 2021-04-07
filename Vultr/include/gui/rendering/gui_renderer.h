#pragma once
#include <set>
#include <unordered_map>
#include <map>
#include "gui_vertex.h"
#include <gui/rendering/quad.h>
#include <rendering/models/vertex_array.h>
#include <rendering/models/vertex_buffer.h>
#include <rendering/models/index_buffer.h>
#include <rendering/models/texture.h>
#include <rendering/models/shader.h>
#include <helpers/texture_importer.h>
#define MAX_QUADS 1000
#define MAX_TEXTURES 16

namespace Vultr
{
namespace GUI
{
struct VertexRange
{
    VertexRange() : begin(0), end(MAX_QUADS * 4)
    {
    }
    VertexRange(VertexID p_begin, VertexID p_end = MAX_QUADS * 4)
        : begin(p_begin), end(p_end)
    {
    }
    VertexID begin;
    VertexID end;

    bool operator==(const VertexRange &other) const
    {
        return this->begin == other.begin && this->end == other.end;
    }

    bool operator<(const VertexRange &other) const
    {
        return this->end < other.begin;
    }
};

class GUIBatch
{
  public:
    GUIBatch() = default;

    bool AddTexture(Texture *texture, unsigned int &slot)
    {
        if (num_textures < MAX_TEXTURES)
        {
            textures[num_textures] = texture;
            slot = num_textures;
            num_textures++;
            return true;
        }
        return false;
    }

    void Bind(Shader *shader)
    {
        for (int i = 0; i < num_textures; i++)
        {
            textures[i]->Bind(GL_TEXTURE0 + i);
        }
    }

  private:
    unsigned int num_textures = 0;
    Texture *textures[MAX_TEXTURES];
};
class GUIRenderer
{
  public:
    GUIRenderer()
    {
        vao = new VertexArray();
        vao->Bind();
        vbo = new VertexBuffer(sizeof(GUI::GUIVertex) * MAX_QUADS * 4);
        vbo->Bind();
        vao->Setup<GUI::GUIVertex>();
        int offset = 0;
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

        for (int i = 0; i < MAX_TEXTURES; i++)
        {
            samplers[i] = i;
        }

        ibo = new IndexBuffer(indices, MAX_QUADS * 6);
        z_index_ranges = {};
    }

    void Draw(Shader *shader)
    {
        if (NeedsReorder())
        {
            OrderBatches();
        }
        for (auto [range, batch] : batches)
        {
            int end = range.end;
            if (end > num_vertices)
            {
                end = num_vertices;
            }
            glUniform1iv(shader->GetUniformLocation("u_Textures"), MAX_TEXTURES,
                         samplers);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            batch.Bind(shader);
            vbo->Bind();
            glBufferSubData(GL_ARRAY_BUFFER, range.begin * sizeof(GUIVertex),
                            (end - range.begin) * sizeof(GUIVertex), vertices);
            vao->Bind();
            ibo->Bind();
            glDrawElements(GL_TRIANGLES, (end - range.begin) / 4 * 6,
                           GL_UNSIGNED_SHORT, (void *)0);
        }
    }

    void OrderBatches()
    {
        batches.clear();
        VertexRange range = VertexRange(0, MAX_QUADS * 4);
        GUIBatch batch = GUIBatch();
        for (VertexID vertex = 0; vertex < num_vertices; vertex += 4)
        {
            assert(vertex_to_quad.find(vertex) != vertex_to_quad.end() &&
                   "Quad does not exist!");
            QuadID quad = vertex_to_quad[vertex];
            // If there is a texture attached
            if (quad_to_texture.find(quad) != quad_to_texture.end())
            {
                Texture *texture = quad_to_texture[quad];
                unsigned int slot = 0;
                if (!batch.AddTexture(texture, slot))
                {
                    batches[range] = batch;
                    range = VertexRange(range.end, MAX_QUADS * 4);
                    batch = GUIBatch();
                    batch.AddTexture(texture, slot);
                    for (int i = vertex; i < vertex + 4; i++)
                    {
                        vertices[i].texture = slot;
                    }
                    continue;
                }
                else
                {
                    for (int i = vertex; i < vertex + 4; i++)
                    {
                        vertices[i].texture = slot;
                    }
                }
            }
            if (vertex >= num_vertices - 4)
            {
                batches[range] = batch;
            }
            else
            {
                range.end += 4;
            }
        }
        needs_reorder = false;
    }

    void SubmitTexture(QuadID quad, Texture *texture)
    {
        quad_to_texture[quad] = texture;
        MarkNeedsReorder();
    }

    void DeleteQuad(QuadID quad)
    {
        assert(quad_to_vertex.find(quad) != quad_to_vertex.end() &&
               "Quad does not exist");
        DeleteVertices(quad_to_vertex[quad], 4);
        if (quad_to_texture.find(quad) != quad_to_texture.end())
        {
            quad_to_texture.erase(quad);
        }
        MarkNeedsReorder();
    }

    Quad GetQuad(QuadID quad)
    {
        return Quad(quad_to_texture.find(quad) != quad_to_texture.end());
    }

    void CommitVertices(QuadID quad, GUIVertex vertices[4])
    {
        if (quad_to_vertex.find(quad) == quad_to_vertex.end())
        {
            Zindex z = round(-vertices[0].position.z * 100);
            // If we have a place to put this z
            if (z_index_ranges.find(z) != z_index_ranges.end())
            {
                VertexID loc = SubmitVerticesToRange(z, vertices);
                LinkQuad(quad, loc);
            }
            // Otherwise we need to create a new z range
            else
            {
                VertexID loc = SubmitNewVertexInZNewRange(z, vertices);
                LinkQuad(quad, loc);
            }
            MarkNeedsReorder();
        }
        else
        {
            VertexID offset = quad_to_vertex[quad];
            for (int i = 0; i < 4; i++)
            {
                vertices[i].texture = this->vertices[offset].texture;
            }
            if (round(this->vertices[offset].position.z * 100) !=
                round(vertices[0].position.z * 100))
            {
                DeleteVertices(offset, 4);
                Zindex z = round(-vertices[0].position.z * 100);
                // If we have a place to put this z
                if (z_index_ranges.find(z) != z_index_ranges.end())
                {
                    VertexID loc = SubmitVerticesToRange(z, vertices);
                    LinkQuad(quad, loc);
                }
                // Otherwise we need to create a new z range
                else
                {
                    VertexID loc = SubmitNewVertexInZNewRange(z, vertices);
                    LinkQuad(quad, loc);
                }
                MarkNeedsReorder();
            }
            else
            {
                for (int i = 0; i < 4; i++)
                {
                    this->vertices[i + offset] = vertices[i];
                }
            }
        }
    }

  private:
    void MarkNeedsReorder()
    {
        needs_reorder = true;
    }

    bool NeedsReorder() const
    {
        return needs_reorder;
    }

    VertexID SubmitVerticesToRange(Zindex z, GUIVertex new_vertices[4])
    {
        assert(z_index_ranges.find(z) != z_index_ranges.end() &&
               "Failure to submit to non existent z, please call "
               "SubmitNewVertexInZNewRange if the z index range does not exist");
        VertexID vertex = num_vertices;
        if (z_index_ranges[z].end < MAX_QUADS * 4)
        {
            vertex = z_index_ranges[z].end + 1;
            z_index_ranges[z].end += 4;
            if (vertex < num_vertices - 1)
            {
                ShiftVerticesRight(vertex, num_vertices - 1, 4);
            }
            for (auto [_z, range] : z_index_ranges)
            {
                if (_z > z)
                {
                    z_index_ranges[_z].begin += 4;
                    if (z_index_ranges[_z].end < MAX_QUADS * 4 - 1)
                    {
                        z_index_ranges[_z].end += 4;
                    }
                }
            }
        }
        this->num_vertices += 4;
        for (int i = 0; i < 4; i++)
        {
            this->vertices[vertex + i] = new_vertices[i];
        }
        return vertex;
    }

    VertexID SubmitNewVertexInZNewRange(Zindex new_z, GUIVertex new_vertices[4])
    {
        assert(z_index_ranges.find(new_z) == z_index_ranges.end() &&
               "Failure to create new z range, only use this function when you "
               "don't have a z range");
        if (z_index_ranges.size() == 0)
        {
            z_index_ranges[new_z] = VertexRange();
        }
        else
        {
            Zindex prev_z = -1;
            Zindex after_z = -1;
            for (auto [z, range] : z_index_ranges)
            {
                if (z < new_z)
                {
                    prev_z = z;
                }
                else if (z > new_z)
                {
                    after_z = z;
                }
            }
            if (prev_z != -1 && z_index_ranges[prev_z].end >= MAX_QUADS * 4)
            {
                z_index_ranges[prev_z].end = num_vertices - 1;
            }

            z_index_ranges[new_z] =
                VertexRange(prev_z == -1 ? 0 : z_index_ranges[prev_z].end + 1);
            if (after_z != -1)
            {
                z_index_ranges[new_z].end = z_index_ranges[after_z].begin + 3;
                for (auto [z, range] : z_index_ranges)
                {
                    if (z >= after_z)
                    {
                        z_index_ranges[z].begin += 4;
                        if (range.end < MAX_QUADS * 4)
                        {
                            z_index_ranges[z].end += 4;
                        }
                    }
                }
                ShiftVerticesRight(z_index_ranges[new_z].begin, num_vertices - 1, 4);
            }
        }
        this->num_vertices += 4;
        for (int i = 0; i < 4; i++)
        {
            this->vertices[z_index_ranges[new_z].begin + i] = new_vertices[i];
        }
        return z_index_ranges[new_z].begin;
    }

    void ShiftVerticesRight(VertexID offset, VertexID end, unsigned int amount)
    {
        assert(end + amount < MAX_QUADS * 4 && "Cannot shift vertices past end");
        for (VertexID i = end; i >= offset; i--)
        {
            if (i % 4 == 0 && vertex_to_quad.find(i) != vertex_to_quad.end())
                ChangeVertexLocation(vertex_to_quad[i], i + amount);
            vertices[i + amount] = vertices[i];
        }
    }
    void ShiftVerticesLeft(VertexID offset, VertexID end, unsigned int amount)
    {
        assert(offset - amount >= 0 && "Cannot shift vertices past end");
        for (VertexID i = offset; i < end + 1; i++)
        {
            if (i % 4 == 0 && vertex_to_quad.find(i) != vertex_to_quad.end())
                ChangeVertexLocation(vertex_to_quad[i], i - amount);
            vertices[i - amount] = vertices[i];
        }
    }

    VertexID ChangeVertexLocation(QuadID quad, VertexID new_vertex_loc)
    {
        VertexID old = quad_to_vertex[quad];
        quad_to_vertex[quad] = new_vertex_loc;
        vertex_to_quad.erase(old);
        vertex_to_quad[new_vertex_loc] = quad;
        return old;
    }

    void DeleteVertices(VertexID vertex, unsigned int amount)
    {
        Zindex old_z = round(-vertices[vertex].position.z * 100);
        if (z_index_ranges.find(old_z) != z_index_ranges.end())
        {
            if (z_index_ranges[old_z].begin ==
                z_index_ranges[old_z].end - amount + 1)
            {
                z_index_ranges.erase(old_z);
            }
            else if (z_index_ranges[old_z].end < MAX_QUADS * 4)
            {
                z_index_ranges[old_z].end -= amount;
            }
        }
        for (auto [z, range] : z_index_ranges)
        {
            if (z > old_z)
            {
                z_index_ranges[z].begin -= amount;
                if (z_index_ranges[z].end < MAX_QUADS * 4)
                {
                    z_index_ranges[z].end -= amount;
                }
            }
        }
        QuadID quad = vertex_to_quad[vertex];
        quad_to_vertex.erase(quad);
        vertex_to_quad.erase(vertex);
        ShiftVerticesLeft(vertex + amount, num_vertices - 1, amount);
        num_vertices -= amount;
    }

    void LinkQuad(QuadID quad, VertexID vertex)
    {
        vertex_to_quad[vertex] = quad;
        quad_to_vertex[quad] = vertex;
    }

    GUIVertex vertices[MAX_QUADS * 4];
    unsigned int num_vertices = 0;
    bool needs_reorder = true;
    std::unordered_map<QuadID, VertexID> quad_to_vertex;
    std::unordered_map<VertexID, QuadID> vertex_to_quad;
    std::unordered_map<QuadID, Texture *> quad_to_texture;
    std::map<Zindex, VertexRange> z_index_ranges;
    std::map<VertexRange, GUIBatch> batches;

    // Opengl shit
    VertexArray *vao;
    VertexBuffer *vbo;
    IndexBuffer *ibo;
    unsigned short indices[MAX_QUADS * 6];
    int samplers[MAX_TEXTURES];
};

} // namespace GUI
} // namespace Vultr

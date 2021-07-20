#include <rendering/types/mesh.h>

namespace Vultr
{
    template <>
    void setup_vertex_array<Vertex>()
    {
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)offsetof(Vertex, position));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)offsetof(Vertex, normal));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)offsetof(Vertex, uv));
    }
    Mesh::Mesh()
    {
    }

    Mesh::~Mesh()
    {
        delete_vertex_array(vao);
        delete_index_buffer(ibo);
        delete_vertex_buffer(vbo);
    }

    void Mesh::Init(const std::vector<Vec3> &p_positions, const std::vector<Vec2> &p_uvs, const std::vector<Vec3> &p_normals, const std::vector<unsigned short> &p_indices)
    {
        this->m_vertices.clear();
        this->m_indices = p_indices;
        for (int i = 0; i < p_positions.size(); i++)
        {
            this->m_vertices.push_back(Vertex(p_positions.at(i), p_normals.at(i), p_uvs.at(i)));
        }

        vao = new_vertex_array();
        bind_vertex_array(vao);
        vbo = new_vertex_buffer(&m_vertices[0], m_vertices.size());
        bind_vertex_buffer(vbo);
        setup_vertex_array<Vertex>();
        ibo = new_index_buffer(&m_indices[0], m_indices.size());
    }

    void Mesh::Init(const std::vector<Vertex> &p_vertices, const std::vector<unsigned short> &p_indices)
    {
        this->m_vertices.clear();
        this->m_indices = p_indices;
        this->m_vertices = p_vertices;

        vao = new_vertex_array();
        bind_vertex_array(vao);
        vbo = new_vertex_buffer(&m_vertices[0], m_vertices.size());
        bind_vertex_buffer(vbo);
        setup_vertex_array<Vertex>();
        ibo = new_index_buffer(&m_indices[0], m_indices.size());
    }

    void Mesh::Draw() const
    {
        bind_vertex_array(vao);
        bind_index_buffer(ibo);

        glDrawElements(GL_TRIANGLES, this->m_indices.size(), GL_UNSIGNED_SHORT, (void *)0);
    }

} // namespace Vultr

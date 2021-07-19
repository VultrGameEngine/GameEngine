#include <rendering/types/mesh.h>

namespace Vultr
{
template <> void VertexArray::Setup<Vertex>()
{
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (const void *)offsetof(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (const void *)offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (const void *)offsetof(Vertex, uv));
}
Mesh::Mesh()
{
}

Mesh::~Mesh()
{
    delete vao;
    delete ibo;
    delete vbo;
}

void Mesh::Init(const std::vector<Vec3> &p_positions,
                const std::vector<Vec2> &p_uvs,
                const std::vector<Vec3> &p_normals,
                const std::vector<unsigned short> &p_indices)
{
    this->m_vertices.clear();
    this->m_indices = p_indices;
    for (int i = 0; i < p_positions.size(); i++)
    {
        this->m_vertices.push_back(
            Vertex(p_positions.at(i), p_normals.at(i), p_uvs.at(i)));
    }

    vao = new VertexArray();
    vao->Bind();
    vbo = new VertexBuffer(&m_vertices[0], m_vertices.size());
    vbo->Bind();
    vao->Setup<Vertex>();
    ibo = new IndexBuffer(&m_indices[0], m_indices.size());
}

void Mesh::Init(const std::vector<Vertex> &p_vertices,
                const std::vector<unsigned short> &p_indices)
{
    this->m_vertices.clear();
    this->m_indices = p_indices;
    this->m_vertices = p_vertices;

    vao = new VertexArray();
    vao->Bind();
    vbo = new VertexBuffer(&m_vertices[0], m_vertices.size());
    vbo->Bind();
    vao->Setup<Vertex>();
    ibo = new IndexBuffer(&m_indices[0], m_indices.size());
}

void Mesh::Draw() const
{
    vao->Bind();
    ibo->Bind();

    glDrawElements(GL_TRIANGLES, this->m_indices.size(), GL_UNSIGNED_SHORT,
                   (void *)0);
}

} // namespace Vultr

#include <rendering/types/mesh.h>

namespace Vultr
{

    bool is_valid_mesh(const Mesh &mesh)
    {
        return mesh.vertices != nullptr;
    }

    Mesh new_mesh(Vec3 positions[], Vec2 uvs[], Vec3 normals[], size_t vertex_count, unsigned short indices[], size_t index_count)
    {
        auto *vertices = static_cast<Vertex *>(malloc(sizeof(Vertex) * vertex_count));

        for (size_t i = 0; i < vertex_count; i++)
        {
            vertices[i] = Vertex(positions[i], normals[i], uvs[i]);
        }

        free(positions);
        free(uvs);
        free(normals);

        auto vao = new_vertex_array();
        bind_vertex_array(vao);

        auto vbo = new_vertex_buffer(vertices, vertex_count);
        bind_vertex_buffer(vbo);

        setup_vertex_array<Vertex>();

        auto ibo = new_index_buffer(indices, index_count);
        return {
            .vertices = vertices,
            .vertex_count = vertex_count,
            .indices = indices,
            .index_count = index_count,
            .vao = vao,
            .ibo = ibo,
            .vbo = vbo,
        };
    }

    Mesh new_mesh(Vertex vertices[], size_t vertex_count, unsigned short indices[], size_t index_count)
    {
        auto vao = new_vertex_array();
        bind_vertex_array(vao);
        auto vbo = new_vertex_buffer(vertices, vertex_count);
        bind_vertex_buffer(vbo);
        setup_vertex_array<Vertex>();
        auto ibo = new_index_buffer(indices, index_count);

        return {
            .vertices = vertices,
            .vertex_count = vertex_count,
            .indices = indices,
            .index_count = index_count,
            .vao = vao,
            .ibo = ibo,
            .vbo = vbo,
        };
    }

    void delete_mesh(Mesh &mesh)
    {
        free(mesh.vertices);
        free(mesh.indices);
        mesh.vertices = nullptr;
        mesh.vertex_count = 0;
        mesh.indices = nullptr;
        mesh.index_count = 0;

        delete_vertex_array(mesh.vao);
        delete_index_buffer(mesh.ibo);
        delete_vertex_buffer(mesh.vbo);
    }

    void draw_mesh(const Mesh &mesh)
    {
        bind_vertex_array(mesh.vao);
        bind_index_buffer(mesh.ibo);

        glDrawElements(GL_TRIANGLES, mesh.index_count, GL_UNSIGNED_SHORT, (void *)0);
    }
} // namespace Vultr

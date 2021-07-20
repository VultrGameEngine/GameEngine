#pragma once
#include <glm/glm.hpp>
#include <helpers/loading_state.h>
#include <rendering/types/index_buffer.h>
#include <rendering/types/shader.h>
#include <rendering/types/vertex.h>
#include <rendering/types/vertex_array.h>
#include <rendering/types/vertex_buffer.h>
#include <vector>
#include <fundamental/types.h>

namespace Vultr
{
    struct Mesh
    {
        // Holds the saved vertices and indices in a buffer cpu side
        Vertex *vertices = nullptr;
        size_t vertex_count = 0;
        unsigned short *indices = nullptr;
        size_t index_count = 0;

        // A bunch of required buffers
        VertexArray vao = 0;
        IndexBuffer ibo = 0;
        VertexBuffer vbo = 0;
    };

    bool is_valid_mesh(const Mesh &mesh);
    Mesh new_mesh(Vec3 positions[], Vec2 uvs[], Vec3 normals[], size_t vertex_count, unsigned short indices[], size_t index_count);
    Mesh new_mesh(Vertex vertices[], size_t vertex_count, unsigned short indices[], size_t index_count);
    void delete_mesh(Mesh &mesh);

    void draw_mesh(const Mesh &mesh);
} // namespace Vultr

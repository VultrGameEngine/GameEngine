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
    class Mesh
    {
      public:
        Mesh();
        ~Mesh();

        inline LoadingState GetLoadingState()
        {
            if (vao == 0 || ibo == 0 || vbo == 0)
                return LoadingState_NotLoaded;
            return LoadingState_Loaded;
        }

        void Init(const std::vector<Vec3> &p_positions, const std::vector<Vec2> &p_uvs, const std::vector<Vec3> &p_normals, const std::vector<unsigned short> &p_indices);
        void Init(const std::vector<Vertex> &p_vertices, const std::vector<unsigned short> &p_indices);

        void Draw() const;

      private:
        // Holds the saved vertices and indices in a buffer cpu side
        std::vector<Vertex> m_vertices;
        std::vector<unsigned short> m_indices;

        // A bunch of required buffers
        VertexArray vao = 0;
        IndexBuffer ibo = 0;
        VertexBuffer vbo = 0;
    };
} // namespace Vultr

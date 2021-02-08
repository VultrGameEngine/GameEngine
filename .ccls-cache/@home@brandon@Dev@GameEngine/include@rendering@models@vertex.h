#pragma once
#include <cstring>
#include <glm/glm.hpp>

namespace Brick3D
{

struct Vertex
{

    // Vertex properties that are in the vertex buffer
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;

    Vertex(glm::vec3 p_position = glm::vec3(0, 0, 0),
           glm::vec3 p_normal = glm::vec3(0, 0, 0), glm::vec2 p_uv = glm::vec2(0, 0))
        : position(p_position), normal(p_normal), uv(p_uv)
    {
    }

    bool operator<(const Vertex that) const
    {
        return std::memcmp((void *)this, (void *)&that, sizeof(Vertex)) > 0;
    };
};

} // namespace Brick3D

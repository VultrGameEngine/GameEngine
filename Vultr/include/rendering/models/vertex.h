#pragma once
#include <cstring>
#include <glm/glm.hpp>
#include <fundamental/types.h>

namespace Vultr
{

    struct Vertex
    {

        // Vertex properties that are in the vertex buffer
        Vec3 position;
        Vec3 normal;
        Vec2 uv;

        Vertex(Vec3 p_position = Vec3(0, 0, 0), Vec3 p_normal = Vec3(0, 0, 0), Vec2 p_uv = Vec2(0, 0)) : position(p_position), normal(p_normal), uv(p_uv)
        {
        }

        bool operator<(const Vertex that) const
        {
            return std::memcmp((void *)this, (void *)&that, sizeof(Vertex)) > 0;
        };
    };

} // namespace Vultr

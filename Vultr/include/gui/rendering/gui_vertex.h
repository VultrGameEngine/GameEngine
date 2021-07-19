#pragma once
#include <types/types.hpp>
#include <rendering/types/vertex_array.h>

namespace Vultr
{
    namespace IMGUI
    {
        struct GUIVertex
        {
            Vec3 position = Vec3(0);
            Vec4 color = Vec4(0);
            Vec2 uv = Vec2(0);
            f32 texture = -1;

            GUIVertex() = default;

            GUIVertex(const GUIVertex &other)
            {
                this->position = other.position;
                this->color = other.color;
                this->uv = other.uv;
                this->texture = other.texture;
            }
        };

    } // namespace IMGUI
    template <>
    void VertexArray::Setup<IMGUI::GUIVertex>();
} // namespace Vultr

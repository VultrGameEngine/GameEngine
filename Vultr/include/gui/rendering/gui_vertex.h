#pragma once
#include <rendering/models/vertex_array.h>

namespace Vultr
{
namespace GUI
{
struct GUIVertex
{
    Vec3 position = Vec3(0);
    Vec4 color = Vec4(0);
    Vec2 uv = Vec2(0);
    Vec4 borders = Vec4(0);
    Vec4 border_color = Vec4(0);
    float texture = -1;

    GUIVertex() = default;
    GUIVertex(const GUIVertex &other)
    {
        this->position = other.position;
        this->color = other.color;
        this->uv = other.uv;
        this->borders = other.borders;
        this->border_color = other.border_color;
        this->texture = other.texture;
    }
};
} // namespace GUI
template <> void VertexArray::Setup<GUI::GUIVertex>();
} // namespace Vultr

#pragma once
#include <rendering/models/vertex_array.h>

namespace Vultr
{
namespace GUI
{
struct GUIVertex
{
    glm::vec3 position = glm::vec3(0);
    glm::vec4 color = glm::vec4(0);
    glm::vec2 uv = glm::vec2(0);
    glm::vec4 borders = glm::vec4(0);
    glm::vec4 border_color = glm::vec4(0);
    float texture = -1;

    GUIVertex() = default;
    GUIVertex(const GUIVertex& other) {
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

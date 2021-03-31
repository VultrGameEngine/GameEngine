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
};
} // namespace GUI
template <> void VertexArray::Setup<GUI::GUIVertex>();
} // namespace Vultr

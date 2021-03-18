#pragma once
#include <rendering/models/vertex_array.h>

namespace Vultr
{
namespace GUI
{
struct GUIVertex
{
    glm::vec3 position;
    glm::vec4 color;
    float texture;
};
} // namespace GUI
template <> void VertexArray::Setup<GUI::GUIVertex>();
} // namespace Vultr

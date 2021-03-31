#pragma once
#include "gui_vertex.h"
#include <array>
#include "../framework/size.h"

namespace Vultr
{
namespace GUI
{

using QuadID = int;
using VertexID = int;

struct QuadProperties
{
    QuadProperties() = default;

    glm::vec4 color = glm::vec4(1);
    Size size = Size(0, 0);
    double rotation = 0;
    glm::vec2 uv = glm::vec2(0, 0);
    glm::vec2 uv_dimensions = glm::vec2(0, 0);
    glm::vec2 texture_dimensions = glm::vec2(0, 0);
    glm::vec4 border_widths = glm::vec4(0);
    glm::vec4 border_color = glm::vec4(0);
};

class BuildContext;

class Quad
{
  public:
    Quad() = default;
    Quad(GUIVertex &p_vertex1, GUIVertex &p_vertex2, GUIVertex &p_vertex3,
         GUIVertex &p_vertex4, int p_texture_slot = -1)
        : texture_slot(p_texture_slot)
    {
        vertices[0] = &p_vertex1;
        vertices[1] = &p_vertex2;
        vertices[2] = &p_vertex3;
        vertices[3] = &p_vertex4;
    }

    void Commit(const QuadProperties &properties, BuildContext *context);

  private:
    std::array<GUIVertex *, 4> vertices;
    int texture_slot;
};
} // namespace GUI
} // namespace Vultr

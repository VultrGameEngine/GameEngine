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
using IndexID = int;
using Zindex = int;
using RenderGroupID = unsigned int;

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
    Quad(bool p_has_texture = false) : has_texture(p_has_texture)
    {
    }

    void Commit(QuadID id, const QuadProperties &properties, BuildContext *context);

  private:
    GUIVertex vertices[4];
    bool has_texture = false;
};
} // namespace GUI
} // namespace Vultr

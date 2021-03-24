#pragma once
#include "gui_vertex.h"
#include <array>
#include "../framework/size.h"

namespace Vultr
{
namespace GUI
{

struct QuadProperties
{
    QuadProperties() = default;

    glm::vec4 color = glm::vec4(1);
    Size size = Size(0, 0);
    double rotation = 0;
};

class BuildContext;

class Quad
{
  public:
    Quad(GUIVertex &p_vertex1, GUIVertex &p_vertex2, GUIVertex &p_vertex3,
         GUIVertex &p_vertex4)
    {
        vertices[0] = &p_vertex1;
        vertices[1] = &p_vertex2;
        vertices[2] = &p_vertex3;
        vertices[3] = &p_vertex4;
    }

    void Commit(const QuadProperties &properties, BuildContext *context);

  private:
    std::array<GUIVertex *, 4> vertices;
};
} // namespace GUI
} // namespace Vultr

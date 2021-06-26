#include <gui/layout/constraints.h>

using namespace Vultr;

IMGUI::Constraints IMGUI::loose_constraints(Vec2 size)
{
    return Constraints(0, size.x, 0, size.y);
}
IMGUI::Constraints IMGUI::tight(Vec2 size)
{
    return Constraints(size.x, size.x, size.y, size.y);
}

IMGUI::Constraints IMGUI::generate_loose(const Constraints &c)
{
    return Constraints(0, c.max_width, 0, c.max_height);
}
void IMGUI::generate_constraints(const Constraints &c, Constraints &other)
{
}

Vec2 IMGUI::constraints_max(const Constraints &c)
{
    return Vec2(c.max_width, c.max_height);
}
Vec2 IMGUI::constraints_min(const Constraints &c)
{
    return Vec2(c.min_width, c.min_height);
}

Vec2 IMGUI::get_size_from_constraints(const Constraints &c, Vec2 size);

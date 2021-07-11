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

IMGUI::Constraints IMGUI::generate_constraints(const Constraints &c, const Constraints &other)
{
    Constraints new_constraints = other;
    if (other.min_width < c.min_width)
        new_constraints.min_width = c.min_width;
    if (other.max_width > c.max_width)
        new_constraints.max_width = c.max_width;
    if (other.min_height < c.min_height)
        new_constraints.min_height = c.min_height;
    if (other.max_height > c.max_height)
        new_constraints.max_height = c.max_height;
    return new_constraints;
}

Vec2 IMGUI::constraints_max(const Constraints &c)
{
    assert(c.max_width != INFINITY && c.max_height != INFINITY && "Cannot get dimensions of infinite size");
    return Vec2(c.max_width, c.max_height);
}
Vec2 IMGUI::constraints_min(const Constraints &c)
{
    return Vec2(c.min_width, c.min_height);
}

Vec2 IMGUI::get_size_from_constraints(const Constraints &c, Vec2 size)
{
    if (size.x < c.min_width)
        size.x = c.min_width;
    else if (size.x > c.max_width)
        size.x = c.max_width;

    if (size.y < c.min_height)
        size.y = c.min_height;
    else if (size.y > c.max_height)
        size.y = c.max_height;

    return size;
}

#pragma once
#include <cmath>
#include <glm/glm.hpp>

namespace Vultr
{
namespace GUI
{
struct Size
{
    Size() : width(0), height(0), value(0)
    {
    }

    Size(glm::vec2 dimensions) : width(dimensions.x), height(dimensions.y), value(0)
    {
    }

    Size(double p_width, double p_height) : width(p_width), height(p_height), value(0)
    {
    }

    Size(double p_value) : width(INFINITY), height(INFINITY), value(p_value)
    {
    }

    double width;
    double height;

    bool operator==(const Size &other)
    {
        return width == other.width && height == other.height;
    }
    bool operator!=(const Size &other)
    {
        return !(*this == other);
    }

    // If the size wants to be a flex, it will set width and height to infinity
    double value;
};
} // namespace GUI

} // namespace Vultr

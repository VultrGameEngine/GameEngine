#pragma once

namespace Vultr
{
namespace GUI
{
struct Size
{
    Size() : width(0), height(0)
    {
    }

    Size(double p_width, double p_height) : width(p_width), height(p_height)
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
};
} // namespace GUI

} // namespace Vultr

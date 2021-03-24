#pragma once
#include "size.h"

namespace Vultr
{
namespace GUI
{
struct BoxConstraints
{
    BoxConstraints(double p_min_width, double p_max_width, double p_min_height,
                   double p_max_height)
        : min_width(p_min_width), max_width(p_max_width), min_height(p_min_height),
          max_height(p_max_height)
    {
    }

    static BoxConstraints Loose(Size size)
    {
        return BoxConstraints(0, size.width, 0, size.height);
    }

    static BoxConstraints Tight(Size size)
    {
        return BoxConstraints(size.width, size.width, size.height, size.height);
    }

    Size Max()
    {
        return Size(max_width, max_height);
    }

    Size Min()
    {
        return Size(min_width, min_height);
    }

    Size GetSize(Size size)
    {
        if (size.width < min_width)
            size.width = min_width;
        else if (size.width > max_width)
            size.width = max_width;

        if (size.height < min_height)
            size.height = min_height;
        else if (size.height > max_height)
            size.height = max_height;

        return size;
    }

    BoxConstraints GenerateLoose()
    {
        return BoxConstraints::Loose(Size(max_width, max_height));
    }

    double min_width;
    double max_width;
    double min_height;
    double max_height;
};
} // namespace GUI
} // namespace Vultr

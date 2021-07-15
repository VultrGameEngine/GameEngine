#pragma once
#include <types/types.hpp>

namespace Vultr
{
    namespace IMGUI
    {
        enum struct HorizontalDirection : u32
        {
            LEFT_TO_RIGHT = 0, // Boxes start at the left and are added to the right.
            RIGHT_TO_LEFT = 1, // Boxes start at the right and are added to the left.
        };

        enum struct VerticalDirection : u32
        {
            UP = 0,   // Boxes should start at the bottom and be stacked vertically towards the top.
            DOWN = 1, // Boxes should start at the top and be stacked vertically towards the bottom.
        };
    } // namespace IMGUI
} // namespace Vultr

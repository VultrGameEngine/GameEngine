#pragma once
#include <types/types.hpp>

namespace Vultr
{
    namespace IMGUI
    {
        struct Alignment
        {
            Vec2 value = Vec2(0);

            static Alignment BOTTOM_CENTER();
            static Alignment BOTTOM_LEFT();
            static Alignment BOTTOM_RIGHT();
            static Alignment CENTER();
            static Alignment CENTER_LEFT();
            static Alignment CENTER_RIGHT();
            static Alignment TOP_CENTER();
            static Alignment TOP_LEFT();
            static Alignment TOP_RIGHT();
        };

        // Thxs Flutter, the low level design of your framework is shit but the goals are good :D

        enum struct MainAxisAlignment : u32
        {
            START = 0,         // Place the children as close to the start of the main axis as possible.
            END = 1,           // Place the children as close to the end of the main axis as possible.
            CENTER = 2,        // Place the children as close to the middle of the main axis as possible.
            SPACE_BETWEEN = 3, // Place the free space evenly between the children.
            SPACE_AROUND = 4,  // Place the free space evenly between the children as well as half of that space before and after the first and last child.
            SPACE_EVENLY = 5,  // Place the free space evenly between the children as well as before and after the first and last child.
        };

        enum struct CrossAxisAlignment : u32
        {
            START = 0,   // Place the children with their start edge aligned with the start side of the cross axis.
            END = 1,     // Place the children as close to the end of the cross axis as possible.
            CENTER = 2,  // Place the children so that their centers align with the middle of the cross axis.
            STRETCH = 3, // Require the children to fill the cross axis. This causes the constraints passed to the children to be tight in the cross axis.
        };
    } // namespace IMGUI
} // namespace Vultr

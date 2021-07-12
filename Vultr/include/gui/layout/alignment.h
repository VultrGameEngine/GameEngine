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
    } // namespace IMGUI
} // namespace Vultr

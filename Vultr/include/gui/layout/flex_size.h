#pragma once
#include <types/types.hpp>

namespace Vultr
{
    namespace IMGUI
    {
        enum struct MainAxisSize : u32
        {
            MIN = 0, // Minimize the amount of free space along the main axis, subject to the incoming layout constraints.
            MAX = 1, // Maximize the amount of free space along the main axis, subject to the incoming layout constraints.
        };
    } // namespace IMGUI
} // namespace Vultr

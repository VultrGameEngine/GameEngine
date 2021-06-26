#pragma once
#include <types/types.hpp>
#include <fundamental/types.h>

namespace Vultr
{
    namespace IMGUI
    {
        struct Size
        {
            // If there is no defined size, then it will be Vec2(-1, -1)
            Vec2 value = Vec2(-1);
        };
        static const Size EMPTY_SIZE = {.value = Vec2(-1)};
    } // namespace IMGUI
} // namespace Vultr

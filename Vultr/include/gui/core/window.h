#pragma once
#include <types/types.hpp>

namespace Vultr
{
    namespace IMGUI
    {
        struct Window
        {
            Vec2 dimensions;
            Vec2 position;
        };

        Window new_window();
    } // namespace IMGUI
} // namespace Vultr

#pragma once
#include <types/types.h>

namespace Vultr
{
    namespace IMGUI
    {
        struct EdgeInsets
        {
            f32 left = 0;
            f32 right = 0;
            f32 top = 0;
            f32 bottom = 0;

            static EdgeInsets SYMMETRICAL(f32 horizontal, f32 vertical);
            static EdgeInsets ALL(f32 all);
        };

        bool has_insets(const EdgeInsets &insets);

    } // namespace IMGUI
} // namespace Vultr

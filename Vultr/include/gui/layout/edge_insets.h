#pragma once
#include <types/types.hpp>

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
        };

        EdgeInsets edge_insets_symmetrical(f32 horizontal, f32 vertical);
        EdgeInsets edge_insets_all(f32 all);

    } // namespace IMGUI
} // namespace Vultr

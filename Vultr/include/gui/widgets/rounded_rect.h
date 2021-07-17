#pragma once
#include <gui/core/context.h>

namespace Vultr
{
    namespace IMGUI
    {
        struct RoundedRectStyle
        {
            Color color = Color(255);

            f32 corner_radius = 0;
        };

        void rounded_rect(Context *c, UI_ID id, RoundedRectStyle style = {});
    } // namespace IMGUI
} // namespace Vultr

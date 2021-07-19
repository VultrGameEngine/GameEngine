#pragma once
#include <gui/core/context.h>

namespace Vultr
{
    namespace IMGUI
    {
        struct RoundedRectStyle
        {
            f32 corner_radius = 0;
        };

        struct RoundedRectState
        {
            RoundedRectStyle style;
        };

        void begin_rounded_rect(Context *c, UI_ID id, RoundedRectStyle style = {});
        void end_rounded_rect(Context *c);
    } // namespace IMGUI
} // namespace Vultr

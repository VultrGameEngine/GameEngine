#pragma once
#include <gui/core/context.h>
#include <gui/layout/constraints.h>

namespace Vultr
{
    namespace IMGUI
    {
        struct ConstrainedBoxState
        {
        };

        struct ConstrainedBoxStyle
        {
            f32 min_width = 0;
            f32 max_width = FLT_MAX;
            f32 min_height = 0;
            f32 max_height = FLT_MAX;
        };

        void begin_constrained_box(Context *c, UI_ID id, const Constraints &style = Constraints());
        void end_constrained_box(Context *c);

    } // namespace IMGUI
} // namespace Vultr

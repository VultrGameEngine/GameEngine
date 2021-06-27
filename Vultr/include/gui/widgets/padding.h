#pragma once
#include <gui/core/context.h>
#include <gui/layout/edge_insets.h>

namespace Vultr
{
    namespace IMGUI
    {
        struct PaddingStyle
        {
            EdgeInsets insets;
        };

        struct PaddingState
        {
            PaddingStyle style;
        };

        void begin_padding(Context *c, UI_ID id, PaddingStyle style);
        void end_padding(Context *c);
    } // namespace IMGUI
} // namespace Vultr

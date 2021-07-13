#pragma once
#include <gui/core/context.h>
#include <gui/layout/alignment.h>

namespace Vultr
{
    namespace IMGUI
    {
        struct StackStyle
        {
            Alignment alignment = Alignment::CENTER();
        };

        struct StackState
        {
            StackStyle style;
        };

        void begin_stack(Context *c, UI_ID id, StackStyle style = {});
        void end_stack(Context *c);
    } // namespace IMGUI
} // namespace Vultr

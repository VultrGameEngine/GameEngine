#pragma once
#include <gui/core/context.h>

namespace Vultr
{
    namespace IMGUI
    {
        struct RowState
        {
        };

        struct RowStyle
        {
        };

        void begin_row(Context *c, UI_ID id);
        void end_row(Context *c);
    } // namespace IMGUI
} // namespace Vultr

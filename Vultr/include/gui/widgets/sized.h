#pragma once
#include <gui/core/context.h>

namespace Vultr
{
    namespace IMGUI
    {
        struct SizedStyle
        {
            Vec2 size;
        };
        void begin_sized(Context *c, UI_ID id, SizedStyle style);
        void end_sized(Context *c);
    } // namespace IMGUI
} // namespace Vultr

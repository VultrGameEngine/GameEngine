#pragma once
#include <gui/core/context.h>

namespace Vultr
{
    namespace IMGUI
    {
        void begin_sized(Context *c, UI_ID id, Vec2 size);
        void end_sized(Context *c);
    } // namespace IMGUI
} // namespace Vultr

#pragma once
#include <gui/core/context.h>
#include <core/models/color.h>

namespace Vultr
{
    namespace IMGUI
    {
        struct ButtonState
        {
            Vec2 shadow_offset = Vec2(20);
        };

        bool button(Context *c, UI_ID id, Color color, Vec2 size, Vec2 position);

    } // namespace IMGUI
} // namespace Vultr

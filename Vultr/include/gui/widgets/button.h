#pragma once
#include <gui/core/context.h>
#include <core/models/color.h>
#include <gui/layout/size.h>

namespace Vultr
{
    namespace IMGUI
    {
        struct ButtonState
        {
            Vec2 scale = Vec2(1);
        };

        struct ButtonStyle
        {
            Size size = EMPTY_SIZE;
            Color color = Color(255);
        };

        bool button(Context *c, UI_ID id, ButtonStyle style);

    } // namespace IMGUI
} // namespace Vultr

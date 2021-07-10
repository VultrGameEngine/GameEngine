#pragma once
#include <gui/core/context.h>
#include <gui/widgets/text.h>
#include <gui/widgets/padding.h>
#include <core/system_providers/input_system_provider.h>

namespace Vultr
{
    namespace IMGUI
    {
        struct TextInputEvent
        {
            enum Type
            {
                BACKSPACE,
                DELETE,
                ENTER,
                MOVE_LEFT,
                MOVE_RIGHT,
                KEY,
            };
            Type type;
            char data = ' ';
        };

        struct TextInputState
        {
            std::queue<TextInputEvent> queued_events;
            Vec2 cursor_pos = Vec2(0);
            Vec2 cursor_drawn_pos = Vec2(0);
        };

        struct TextInputStyle
        {
            Color background_color = Color(255);
            Color cursor_color = Color(0, 0, 255, 255);
            TextStyle text_style = {};
            PaddingStyle padding_style = {.insets = edge_insets_all(0)};
            u32 max_lines = 1;
            s32 max_characters = -1;
        };

        void text_input(Context *c, UI_ID id, std::string &text, TextInputStyle style = {});
    } // namespace IMGUI
} // namespace Vultr

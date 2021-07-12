#pragma once
#include <gui/core/context.h>
#include <core/models/color.h>
#include <gui/layout/size.h>
#include <gui/layout/edge_insets.h>
#include <gui/widgets/text.h>

namespace Vultr
{
    namespace IMGUI
    {
        struct ButtonStyle
        {
            Color color = Color(255);
        };

        struct TextButtonStyle
        {
            Color background_color = Color(0, 0, 255, 255);
            TextStyle text_style = {
                .font_color = Color(255),
                .font_size = 12,
                .line_spacing = 1,
                .highlight_color = Color(0),
            };
            EdgeInsets padding = EdgeInsets::SYMMETRICAL(100, 50);
        }; // namespace IMGUI

        struct TextButtonState
        {
            std::string text;
        };

        struct ImageButtonState
        {
        };

        struct ButtonState
        {
            Vec2 scale = Vec2(1);
            ButtonStyle style;
            enum Type
            {
                GENERIC,
                TEXT,
                IMAGE,
            };
        };

        bool text_button(Context *c, UI_ID id, std::string text, TextButtonStyle style = TextButtonStyle());
        void begin_button(Context *c, UI_ID id, ButtonStyle style = ButtonStyle());
        bool end_button(Context *c);

    } // namespace IMGUI
} // namespace Vultr

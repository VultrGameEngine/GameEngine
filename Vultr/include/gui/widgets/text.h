#pragma once
#include <gui/core/context.h>
#include <core/models/color.h>
#include <gui/rendering/quad_batch.h>

namespace Vultr
{
    namespace IMGUI
    {
        struct TextState
        {
            std::string text;
            QuadBatch *batch;
            Vec2 size;
        };

        struct TextStyle
        {
            Color font_color = Color(0, 0, 0, 255);
            f32 font_size = 12;
            f32 line_spacing = 1;
            Color highlight_color = Color(0);
            u32 max_lines = 1;

            enum TextAlign
            {
                TEXT_ALIGN_LEFT,
                TEXT_ALIGN_CENTER,
                TEXT_ALIGN_RIGHT,
            };
        };

        f32 text_get_height(Context *c, const TextStyle &style);
        void text(Context *c, UI_ID id, const std::string &text, TextStyle style = {});

    } // namespace IMGUI
} // namespace Vultr

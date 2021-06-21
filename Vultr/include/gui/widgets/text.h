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
        };

        struct TextParams
        {
            f32 font_size = 12;
            f32 line_spacing = 1;
            Color highlight_color = Color(0);
            Color color = Color(0, 0, 0, 255);
        };

        void text(Context *c, UI_ID id, std::string text, Vec2 position, TextParams params);

    } // namespace IMGUI
} // namespace Vultr

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

        void text(Context *c, UI_ID id, std::string text, Color color, Vec2 size, Vec2 position);

    } // namespace IMGUI
} // namespace Vultr

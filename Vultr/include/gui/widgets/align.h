#pragma once
#include <gui/core/context.h>
#include <gui/layout/alignment.h>

namespace Vultr
{
    namespace IMGUI
    {
        struct AlignState
        {
            Alignment alignment = Alignment::CENTER();
        };

        // Get the relative position of a child within a parent given an alignment
        Vec2 get_alignment_position(Alignment alignment, const Vec2 &child_size, const Vec2 &parent_size);

        void begin_align(Context *c, UI_ID id, Alignment alignment);
        void end_align(Context *c);
    } // namespace IMGUI
} // namespace Vultr

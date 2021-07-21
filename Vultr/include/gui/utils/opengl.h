#pragma once
#include <gui/core/context.h>
#include <core/models/color.h>

namespace Vultr
{
    namespace IMGUI
    {
        Vec2 gl_get_size(Context *c, Vec2 size);
        Vec4 gl_get_color(Color color);
        Vec2 gl_get_raw_position(Context *c, Vec2 position);
        Vec2 gl_get_position(Context *c, Vec2 position, Vec2 gl_size);

        Vec2 screen_get_size_from_gl(Context *c, Vec2 size);
        Vec2 screen_get_position_from_gl(Context *c, Vec2 position, Vec2 gl_size);
    } // namespace IMGUI
} // namespace Vultr

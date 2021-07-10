#pragma once
#include <core/models/color.h>

namespace Vultr
{
    namespace IMGUI
    {
        Vec2 gl_get_size(Vec2 size);
        Vec4 gl_get_color(Color color);
        Vec2 gl_get_raw_position(Vec2 position);
        Vec2 gl_get_position(Vec2 position, Vec2 gl_size);

        Vec2 screen_get_size_from_gl(Vec2 size);
        Vec2 screen_get_position_from_gl(Vec2 position, Vec2 gl_size);
    } // namespace IMGUI
} // namespace Vultr

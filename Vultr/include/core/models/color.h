#pragma once
#include <fundamental/types.h>
#include <types/types.h>

namespace Vultr
{
    struct Color
    {
        Color() : value(Vec4(0, 0, 0, 1)){};
        Color(f32 r, f32 g, f32 b, f32 a = 255) : value(Vec4(r, g, b, a)){};
        Color(f32 rgba) : value(Vec4(rgba)){};
        Color(f32 rgb, f32 a) : value(Vec4(rgb, rgb, rgb, a)){};
        Color(Vec4 p_value) : value(p_value){};
        Vec4 value;
    };
} // namespace Vultr

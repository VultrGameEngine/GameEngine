#include <gui/utils/opengl.h>
#include <core/system_providers/render_system_provider.h>

using namespace Vultr;
Vec2 IMGUI::gl_get_size(Context *c, Vec2 size)
{
    return size / c->window.dimensions;
}

Vec4 IMGUI::gl_get_color(Color color)
{
    return color.value / Vec4(255);
}

Vec2 IMGUI::gl_get_raw_position(Context *c, Vec2 position)
{
    auto pos = position / c->window.dimensions * Vec2(2);
    pos.y = -pos.y;
    return pos;
}

Vec2 IMGUI::gl_get_position(Context *c, Vec2 position, Vec2 gl_size)
{
    auto pos = position / c->window.dimensions * Vec2(2) + Vec2(-1, 1);
    pos.y = 2 - pos.y;
    pos += gl_size * Vec2(1, -1);
    return pos;
}

Vec2 IMGUI::screen_get_size_from_gl(Context *c, Vec2 size)
{
    return size * c->window.dimensions;
}

Vec2 IMGUI::screen_get_position_from_gl(Context *c, Vec2 position, Vec2 gl_size)
{
    auto pos = position - gl_size * Vec2(1, -1);
    pos.y = -1 * (pos.y - 2);
    pos = (pos - Vec2(-1, 1)) / Vec2(2) * c->window.dimensions;
    return pos;
}

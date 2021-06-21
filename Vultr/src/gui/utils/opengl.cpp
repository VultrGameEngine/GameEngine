#include <gui/utils/opengl.h>
#include <core/system_providers/render_system_provider.h>

using namespace Vultr;
Vec2 IMGUI::gl_get_size(Vec2 size)
{
    return size / RenderSystem::get_dimensions(GAME);
}

Vec4 IMGUI::gl_get_color(Color color)
{
    return color.value / Vec4(255);
}

Vec2 IMGUI::gl_get_position(Vec2 position, Vec2 size)
{
    auto pos = position / RenderSystem::get_dimensions(GAME) * Vec2(2) + Vec2(-1, 1);
    pos.y = 2 - pos.y;
    pos += size * Vec2(1, -1);
    return pos;
}

#include <gui/rendering/quad.h>
#include <gui/core/context.h>
#include <gui/utils/opengl.h>

using namespace Vultr;

void IMGUI::set_quad_transform(Context *c, Quad &quad, Vec2 position, Vec2 scale)
{
    auto size = gl_get_size(c, scale);
    auto pos = gl_get_position(c, position, size);
    glm::mat4 transform = glm::translate(Vec3(pos, 0)) * glm::scale(Vec3(size, 1));

    for (s32 i = 0; i < 4; i++)
    {
        auto &v = quad.vertices[i];
        auto rp = transform * Vec4(v.position, 1.0);
        v.position = Vec3(rp.x, rp.y, rp.z);
    }
}

void IMGUI::set_quad_texture_slot(Context *c, Quad &quad, f32 texture_slot)
{
    for (s32 i = 0; i < 4; i++)
    {
        quad.vertices[i].texture = texture_slot;
    }
}

void IMGUI::set_quad_color(Context *c, Quad &quad, Color color)
{
    for (s32 i = 0; i < 4; i++)
    {
        quad.vertices[i].color = color.value / Vec4(255);
    }
}

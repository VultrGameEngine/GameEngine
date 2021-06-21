#include <gui/widgets/button.h>
#include <gui/widgets/text.h>
#include <math/lerp.h>

using namespace Vultr;

#define __button_cache_id ui_id(__FILE__)

bool IMGUI::button(Context *c, UI_ID id)
{
}

bool IMGUI::button(Context *c, UI_ID id, Color color, Vec2 position, Vec2 size)
{
    auto &state = get_widget_cache<ButtonState>(c, __button_cache_id, id);
    auto result = false;
    if (is_active(c, id))
    {
        if (c->left_mb.mouse_went_up)
        {
            if (is_hot(c, id))
            {
                result = true;
            }
            c->active = NO_ID;
        }
    }
    else if (is_hot(c, id))
    {
        if (c->left_mb.mouse_went_down)
        {
            set_active(c, id);
        }
    }

    if (mouse_over(position, size))
    {
        c->hot = id;
    }
    else
    {
        set_not_hot(c, id);
    }

    Vec2 scale;
    if (is_active(c, id))
    {
        scale = Vec2(0.97);
    }
    else if (is_hot(c, id))
    {
        scale = Vec2(1.03);
    }
    else
    {
        scale = Vec2(1);
    }

    state.scale = Math::lerp(state.scale, scale, 0.000000001, c->delta_time);

    Vec2 animated_size = glm::scale(Vec3(state.scale, 1.0)) * Vec4(size, 0.0, 1.0);

    Vec2 difference = (animated_size - size) / Vec2(2);

    // draw_rect(c, Vec4(59, 59, 59, 50), position, size);
    draw_rect(c, color.value, position - difference, animated_size);
    return result;
}

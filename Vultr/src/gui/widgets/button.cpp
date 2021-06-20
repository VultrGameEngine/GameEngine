#include <gui/widgets/button.h>
#include <math/lerp.h>

using namespace Vultr;

#define __button_cache_id ui_id(__FILE__)

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

    Vec2 shadow_offset;
    if (is_active(c, id))
    {
        shadow_offset = Vec2(0);
    }
    else if (is_hot(c, id))
    {
        shadow_offset = Vec2(30);
    }
    else
    {
        shadow_offset = Vec2(20);
    }

    state.shadow_offset = Math::lerp(state.shadow_offset, shadow_offset, 0.000000001, c->delta_time);

    draw_rect(c, Vec4(59, 59, 59, 50), position + state.shadow_offset, size);
    draw_rect(c, color.value, position, size);
    return result;
}

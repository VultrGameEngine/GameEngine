#include <gui/widgets/button.h>
#include <gui/widgets/text.h>
#include <gui/widgets/padding.h>
#include <gui/materials/default_gui_material.h>
#include <math/lerp.h>
#include <core/system_providers/render_system_provider.h>

using namespace Vultr;

#define __button_cache_id ui_id(__FILE__)

void IMGUI::begin_button(Context *c, UI_ID id, ButtonStyle style)
{
    auto &state = get_widget_cache<ButtonState>(c, __button_cache_id, id);
    state.style = style;
    auto parent_constraints = get_constraints(c, id);
    auto scl = new_single_child_layout(id, Vec2(0), parent_constraints);
    begin_layout_with_children(c, id, scl);
}

bool IMGUI::end_button(Context *c)
{
    auto &layout = end_layout_with_children(c);

    auto &data = get_layout_data<SingleChildLayout>(layout);
    auto child = data.child;
    auto &child_layout = get_widget_layout(c, child);
    auto child_size = child_layout.local_size;

    auto id = layout.owner;
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

    auto parent_constraints = get_constraints(c, id);
    auto size = child_size;
    layout.local_size = size;

    if (mouse_over(c, id, size))
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
        scale = Vec2(0.93);
    }
    else if (is_hot(c, id))
    {
        scale = Vec2(1.07);
    }
    else
    {
        scale = Vec2(1);
    }

    state.scale = Math::lerp(state.scale, scale, 0.000000001, c->delta_time);

    Vec2 animated_size = glm::scale(Vec3(state.scale, 1.0)) * Vec4(size, 0.0, 1.0);

    Vec2 difference = (animated_size - size) / Vec2(2);

    data.child_transform.position = Vec2(0);
    data.child_transform.scale = state.scale;
    auto *mat = new_gui_material(c, state.style.color);

    draw_rect(c, id, -difference, animated_size, mat);
    return result;
}

bool IMGUI::text_button(Context *c, UI_ID id, std::string text, TextButtonStyle button_style)
{
    begin_button(c, id, {.color = button_style.background_color});
    IMGUI::begin_padding(c, __LINE__, {.insets = button_style.padding});
    {
        IMGUI::text(c, id + 1209 + __LINE__, text, button_style.text_style);
    }
    IMGUI::end_padding(c);
    return end_button(c);
}

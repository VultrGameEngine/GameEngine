#include <gui/widgets/button.h>
#include <gui/widgets/text.h>
#include <gui/widgets/padding.h>
#include <gui/materials/default_gui_material.h>
#include <math/lerp.h>

using namespace Vultr;

#define __button_cache_id ui_id(__FILE__)

template <>
void IMGUI::destroy_cache<IMGUI::ButtonState>(Context *c, ButtonState &cache){};

void IMGUI::begin_button(Context *c, UI_ID id, ButtonStyle style)
{
    auto &state = get_widget_cache<ButtonState>(c, __button_cache_id, id);
    state.style = style;
    auto parent_constraints = get_constraints(c, id);
    auto scl = new_single_child_layout(__button_cache_id, id, Vec2(0), parent_constraints);
    begin_layout_with_children(c, id, scl);
}

bool IMGUI::end_button(Context *c)
{
    Layout &layout = end_layout_with_children(c, __button_cache_id);

    SingleChildLayout &data = get_layout_data<SingleChildLayout>(layout);
    UI_ID child = data.child;
    Layout &child_layout = get_widget_layout(c, child);
    Vec2 child_size = child_layout.local_size;

    UI_ID id = layout.owner;
    ButtonState &state = get_widget_cache<ButtonState>(c, __button_cache_id, id);
    ButtonStyle &style = state.style;
    bool result = false;

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
        scale = Vec2(1 - style.expand_factor);
    }
    else if (is_hot(c, id))
    {
        scale = Vec2(1 + style.expand_factor);
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
    auto *mat = new_gui_material(c, style.color);

    draw_rect(c, id, -difference, animated_size, mat);
    return result;
}

bool IMGUI::text_button(Context *c, UI_ID id, std::string text, TextButtonStyle button_style)
{
    begin_button(c, id,
                 {
                     .color = button_style.background_color,
                     .expand_factor = button_style.expand_factor,
                 });
    IMGUI::begin_padding(c, ui_id("TEXT_BUTTON_PADDING"), {.insets = button_style.padding});
    {
        IMGUI::text(c, id + ui_id("TEXT_BUTTON_TEXT"), text, button_style.text_style);
    }
    IMGUI::end_padding(c);
    return end_button(c);
}

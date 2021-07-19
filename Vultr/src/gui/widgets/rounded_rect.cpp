#include <gui/widgets/rounded_rect.h>
#include <gui/materials/default_rounded_material.h>

using namespace Vultr;

#define __rounded_rect_cache_id ui_id(__FILE__)

template <>
void IMGUI::destroy_cache<IMGUI::RoundedRectState>(RoundedRectState &cache){};

void IMGUI::begin_rounded_rect(Context *c, UI_ID id, RoundedRectStyle style)
{
    auto &state = get_widget_cache<RoundedRectState>(c, __rounded_rect_cache_id, id);
    state.style = style;

    Constraints parent_constraints = get_constraints(c, id);
    auto scl = new_single_child_layout(__rounded_rect_cache_id, id, Vec2(0), parent_constraints);
    begin_layout_with_children(c, id, scl);

    // Start our stencil
    begin_stencil(c, id);
}

void IMGUI::end_rounded_rect(Context *c)
{
    // End our layout
    Layout &layout = end_layout_with_children(c, __rounded_rect_cache_id);
    SingleChildLayout &data = get_layout_data<SingleChildLayout>(layout);
    UI_ID id = layout.owner;

    // Get the parent constraints
    Constraints parent_constraints = get_constraints(c, id);

    // Get the child
    UI_ID child = data.child;
    Layout &child_layout = get_widget_layout(c, child);

    layout.local_size = get_size_from_constraints(parent_constraints, child_layout.local_size);

    // Get our state
    RoundedRectState &state = get_widget_cache<RoundedRectState>(c, __rounded_rect_cache_id, id);
    RoundedRectStyle &style = state.style;

    // The color here doesn't actually do anything, it's just there to keep the rounded material happy
    auto *mat = new_rounded_material(c, Color(255), Vec4(style.corner_radius));
    end_stencil(c, id, Vec2(0), layout.local_size, mat);
}

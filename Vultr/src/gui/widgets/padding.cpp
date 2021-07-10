#include <gui/widgets/padding.h>

using namespace Vultr;

#define __padding_cache_id ui_id(__FILE__)

template <>
void IMGUI::destroy_cache<IMGUI::PaddingState>(PaddingState &cache){};
void IMGUI::begin_padding(Context *c, UI_ID id, PaddingStyle style)
{
    auto &state = get_widget_cache<PaddingState>(c, __padding_cache_id, id);
    state.style = style;

    auto parent_constraints = get_constraints(c, id);

    f32 horizontal_diff = style.insets.left + style.insets.right;
    f32 vertical_diff = style.insets.top + style.insets.bottom;

    auto max = Vec2(parent_constraints.max_width - horizontal_diff, parent_constraints.max_height - vertical_diff);

    Constraints constraints = generate_loose(parent_constraints);
    constraints.max_width = max.x;
    constraints.max_height = max.y;

    auto scl = new_single_child_layout(id, Vec2(0), constraints);

    begin_layout_with_children(c, id, scl);
}

void IMGUI::end_padding(Context *c)
{
    auto &layout = end_layout_with_children(c);
    auto id = layout.owner;

    auto &data = get_layout_data<SingleChildLayout>(layout);

    auto parent_constraints = get_constraints(c, id);

    if (data.child == NO_ID)
    {
        layout.local_size = constraints_max(parent_constraints);
        return;
    }

    data.render_child = true;

    auto &state = get_widget_cache<PaddingState>(c, __padding_cache_id, id);
    auto insets = state.style.insets;

    auto child_size = get_widget_layout(c, data.child).local_size;
    data.child_transform.position = Vec2(insets.left, insets.top);
    layout.local_size = child_size + Vec2(insets.left + insets.right, insets.top + insets.bottom);
}

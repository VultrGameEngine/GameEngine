#include <gui/widgets/center.h>

using namespace Vultr;

void IMGUI::begin_center(IMGUI::Context *c, UI_ID id)
{
    auto parent_constraints = get_constraints(c, id);
    auto scl = new_single_child_layout(id, constraints_max(parent_constraints), generate_loose(parent_constraints));
    begin_layout_with_children(c, id, scl);
}

void IMGUI::end_center(IMGUI::Context *c)
{
    auto &layout = end_layout_with_children(c);
    auto &data = get_layout_data<SingleChildLayout>(layout);
    if (data.child == NO_ID)
        return;
    data.render_child = true;
    auto child_size = get_widget_layout(c, data.child).local_size;
    data.child_transform.position = (layout.local_size - child_size) / Vec2(2);
}

#include <gui/widgets/sized.h>
#include <gui/layout/layout.h>

using namespace Vultr;

#define __sized_cache_id ui_id(__FILE__)

void IMGUI::begin_sized(Context *c, UI_ID id, SizedStyle style)
{
    auto size = style.size;
    auto parent_constraints = get_constraints(c, id);
    auto child_constraints = generate_constraints(parent_constraints, Constraints(size, size));
    auto scl = new_single_child_layout(__sized_cache_id, id, get_size_from_constraints(child_constraints, size), child_constraints);

    auto &data = get_layout_data<SingleChildLayout>(scl);
    data.render_child = true;
    data.child_transform.position = Vec2(0);

    begin_layout_with_children(c, id, scl);
}
void IMGUI::end_sized(Context *c)
{
    end_layout_with_children(c, __sized_cache_id);
}

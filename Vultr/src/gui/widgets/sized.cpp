#include <gui/widgets/sized.h>
#include <gui/layout/layout.h>

using namespace Vultr;

void IMGUI::begin_sized(Context *c, UI_ID id, SizedStyle style)
{
    auto size = style.size;
    auto child_constraints = Constraints(size, size);
    auto scl = new_single_child_layout(id, size, child_constraints);

    auto &data = get_layout_data<SingleChildLayout>(scl);
    data.render_child = true;
    data.child_position = Vec2(0);

    begin_layout_with_children(c, id, scl);
}
void IMGUI::end_sized(Context *c)
{
    end_layout_with_children(c);
}

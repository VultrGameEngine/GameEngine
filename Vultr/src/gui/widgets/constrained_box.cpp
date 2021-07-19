#include <gui/widgets/constrained_box.h>

using namespace Vultr;

#define __constrained_box_cache_id 12

// ui_id(__FILE__)

void IMGUI::begin_constrained_box(Context *c, UI_ID id, const Constraints &style)
{
    // Get the constraints given to us
    auto parent_constraints = get_constraints(c, id);

    // Create new constraints from our style
    auto style_constraints = Constraints(style.min_width, style.max_width, style.min_height, style.max_height);
    auto child_constraints = generate_constraints(parent_constraints, style_constraints);

    // Begin our layout which will be single child
    auto scl = new_single_child_layout(__constrained_box_cache_id, id, Vec2(0), child_constraints);
    begin_layout_with_children(c, id, scl);
}

void IMGUI::end_constrained_box(Context *c)
{
    // Finish our layout and get the data
    auto &layout = end_layout_with_children(c, __constrained_box_cache_id);
    auto &data = get_layout_data<SingleChildLayout>(layout);
    auto id = layout.owner;

    // Get the parent constraints
    auto parent_constraints = get_constraints(c, id);

    // Get our child size data
    auto child = data.child;

    // If no child was rendered to our padding, then nothing needs to be done
    if (data.child == NO_ID || data.child == UNSET_ID)
    {
        layout.local_size = constraints_max(parent_constraints);
        return;
    }

    auto &child_layout = get_widget_layout(c, child);
    auto child_size = child_layout.local_size;

    // Set our size equal to our child's size
    layout.local_size = child_size;
}

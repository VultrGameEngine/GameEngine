#include <gui/widgets/stack.h>
#include <gui/widgets/align.h>

using namespace Vultr;

#define __stack_cache_id ui_id(__FILE__)

template <>
void IMGUI::destroy_cache<IMGUI::StackState>(Context *c, StackState &cache){};

void IMGUI::begin_stack(Context *c, UI_ID id, StackStyle style)
{
    auto &cache = get_widget_cache<StackState>(c, __stack_cache_id, id);
    cache.style = style;

    // Get the parent constraints, these will be the default constraints for all of the widgets
    Constraints parent_constraints = get_constraints(c, id);

    // Create our multi child layout
    Layout mcl = new_multi_child_layout(__stack_cache_id, id, Vec2(0), parent_constraints, Vec2(0));

    // Then begin our layout
    begin_layout_with_children(c, id, mcl);
}

void IMGUI::end_stack(Context *c)
{
    // End our layout
    Layout &layout = end_layout_with_children(c, __stack_cache_id);
    MultiChildLayout &data = get_layout_data<MultiChildLayout>(layout);
    UI_ID id = layout.owner;

    // Get our cache
    auto &cache = get_widget_cache<StackState>(c, __stack_cache_id, id);
    const StackStyle &style = cache.style;

    // Declare our max size
    auto max_size = Vec2(0);

    // Loop through all children to get their max size's
    for (UI_ID child : data.children)
    {
        // Get the layout
        const Layout &child_layout = get_widget_layout(c, child);

        // Get the child size
        Vec2 child_size = child_layout.local_size;

        // Update our max size
        if (child_size.x > max_size.x)
        {
            max_size.x = child_size.x;
        }
        if (child_size.y > max_size.y)
        {
            max_size.y = child_size.y;
        }
    }

    layout.local_size = max_size;

    // Loop through again to set all the children sizes
    for (UI_ID child : data.children)
    {
        // Get the layout
        const Layout &child_layout = get_widget_layout(c, child);

        // Get the child size
        Vec2 child_size = child_layout.local_size;

        // Align the child within the stack
        Vec2 child_position = get_alignment_position(style.alignment, child_size, max_size);

        data.children_transforms[child].position = child_position;
    }
}

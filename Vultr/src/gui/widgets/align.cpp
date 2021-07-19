#include <gui/widgets/align.h>

using namespace Vultr;

#define __align_cache_id ui_id(__FILE__)

template <>
void IMGUI::destroy_cache<IMGUI::AlignState>(AlignState &cache){};

Vec2 IMGUI::get_alignment_position(Alignment alignment, const Vec2 &child_size, const Vec2 &parent_size)
{
    // Get the alignment
    alignment.value += Vec2(1);
    alignment.value /= Vec2(2);

    Vec2 difference = parent_size - child_size;
    return alignment.value * difference;
}

void IMGUI::begin_align(Context *c, UI_ID id, Alignment alignment)
{
    // Get our state cache and put our style in it
    auto &cache = get_widget_cache<AlignState>(c, __align_cache_id, id);
    cache.alignment = alignment;

    // Get the parent constraints
    auto parent_constraints = generate_loose(get_constraints(c, id));

    // The size of our widget will fill the parent widget
    auto size = constraints_max(parent_constraints);

    // Create a single child layout
    auto scl = new_single_child_layout(__align_cache_id, id, size, parent_constraints);

    // And begin our layout
    begin_layout_with_children(c, id, scl);
}

void IMGUI::end_align(Context *c)
{
    // End our layout and get the data
    Layout &layout = end_layout_with_children(c, __align_cache_id);
    SingleChildLayout &data = get_layout_data<SingleChildLayout>(layout);
    UI_ID id = layout.owner;

    // Get the child
    UI_ID child = data.child;

    // If there is no child then there is nothing to do
    if (child == NO_ID)
        return;

    // Get the layout's child
    const Layout &child_layout = get_widget_layout(c, child);

    // Get the size of the child and parent
    Vec2 parent_size = layout.local_size;
    Vec2 child_size = child_layout.local_size;

    // Get our state cache
    auto &cache = get_widget_cache<AlignState>(c, __align_cache_id, id);

    // Align the child
    data.child_transform.position = get_alignment_position(cache.alignment, child_size, parent_size);
}

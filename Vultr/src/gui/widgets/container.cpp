#include <gui/widgets/container.h>
#include <gui/widgets/padding.h>
#include <gui/widgets/constrained_box.h>
#include <gui/materials/default_gui_material.h>

using namespace Vultr;

#define __container_cache_id ui_id(__FILE__)

template <>
void IMGUI::destroy_cache<IMGUI::ContainerState>(ContainerState &cache){};

// Draw a container without a child
void IMGUI::container(Context *c, UI_ID id, ContainerStyle style)
{
    begin_container(c, id, style);
    end_container(c);
}

// Draw container with a single child
void IMGUI::begin_container(Context *c, UI_ID id, ContainerStyle style)
{
    // Get our state cache and put our style in it
    auto &cache = get_widget_cache<ContainerState>(c, __container_cache_id, id);
    cache.style = style;

    // Get the parent constraints
    auto parent_constraints = get_constraints(c, id);

    // Add any margins to our constraints
    auto margins = style.margin;
    auto margin_diff = Vec2(margins.left + margins.right, margins.top + margins.bottom);
    auto margin_constraints = Constraints(Vec2(0), Vec2(parent_constraints.max_width, parent_constraints.max_height) - margin_diff);
    auto child_constraints = generate_constraints(parent_constraints, margin_constraints);

    // Create the layout
    auto scl = new_single_child_layout(__container_cache_id, id, Vec2(0), child_constraints);

    // Leave room for our rectangle
    c->z_index++;

    // And begin our layout
    begin_layout_with_children(c, id, scl);

    // If there is padding, then we will render it here
    begin_padding(c, id + __container_cache_id + __LINE__,
                  {
                      .insets = style.padding,
                  });

    // If there are constraints, then we will render it here
    begin_constrained_box(c, id + __container_cache_id + __LINE__, style.constraints);
}

void IMGUI::end_container(Context *c)
{
    // End our constrainted box
    end_constrained_box(c);

    // End our padding
    end_padding(c);

    // Get the layout and data
    auto &layout = end_layout_with_children(c, __container_cache_id);
    auto &data = get_layout_data<SingleChildLayout>(layout);
    auto id = layout.owner;

    // Get our cache for our container style
    auto &cache = get_widget_cache<ContainerState>(c, __container_cache_id, id);
    auto &style = cache.style;

    // The container will have different behavior if there is a child or not
    auto child = data.child;
    auto child_layout = get_widget_layout(c, child);

    // Get the parent constraints
    auto parent_constraints = get_constraints(c, id);

    // By default, we are just going to set the size of our container to fill our parent
    Vec2 size = get_size_from_constraints(parent_constraints, child_layout.local_size);

    // Set the size of our layout as our child's size, which will be the max if there is no child because of how the padding widget works
    layout.local_size = size;

    // Go down 2 z indices so we can render our rectangle
    c->z_index -= 2;

    // Create our material
    auto &color = style.color;
    auto *mat = new_gui_material(c, color);

    // Draw our rectangle
    draw_rect(c, id, Vec2(0), layout.local_size, mat);
}

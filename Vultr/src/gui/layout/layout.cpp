#include <gui/layout/layout.h>

using namespace Vultr;

IMGUI::Layout::Layout()
{
    this->data.no_child = {};
}

IMGUI::Layout::Layout(const Layout &other)
{
    this->owner = other.owner;
    this->parent = other.parent;
    this->type = other.type;
    this->local_size = other.local_size;
    switch (type)
    {
        case NO_CHILD:
            this->data.no_child = other.data.no_child;
            break;
        case SINGLE_CHILD:
            this->data.single_child = other.data.single_child;
            break;
        case MULTI_CHILD:
            this->data.multi_child = other.data.multi_child;
            break;
    }
}

void IMGUI::Layout::operator=(const Layout &other)
{
    this->owner = other.owner;
    this->parent = other.parent;
    this->type = other.type;
    this->local_size = other.local_size;
    switch (type)
    {
        case NO_CHILD:
            this->data.no_child = other.data.no_child;
            break;
        case SINGLE_CHILD:
            this->data.single_child = other.data.single_child;
            break;
        case MULTI_CHILD:
            this->data.multi_child = other.data.multi_child;
            break;
    }
}

IMGUI::Layout IMGUI::new_no_child_layout(UI_ID owner, Vec2 local_size, UI_ID parent)
{
    Layout l;
    l.type = Layout::NO_CHILD;
    l.data.no_child = {};
    l.owner = owner;
    l.parent = parent;
    l.local_size = local_size;
    return l;
}

IMGUI::Layout IMGUI::new_single_child_layout(UI_ID owner, Vec2 local_size, Constraints constraints, UI_ID parent)
{
    Layout l;
    l.type = Layout::SINGLE_CHILD;
    l.data.single_child = {.child_constraints = constraints};
    l.owner = owner;
    l.parent = parent;
    l.local_size = local_size;
    return l;
}

IMGUI::Layout IMGUI::new_multi_child_layout(UI_ID owner, Vec2 local_size, Constraints default_constraints, Vec2 default_position, UI_ID parent)
{
    Layout l;
    l.type = Layout::MULTI_CHILD;
    l.data.multi_child = {.default_constraints = default_constraints};
    l.data.multi_child.default_transform.position = default_position;
    l.owner = owner;
    l.parent = parent;
    l.local_size = local_size;
    return l;
}

void IMGUI::layout_add_child(Layout &l, UI_ID id)
{
    assert(l.type != Layout::NO_CHILD && "Widget not allowed to have children!");
    if (l.type == Layout::SINGLE_CHILD)
    {
        auto &layout_data = get_layout_data<SingleChildLayout>(l);
        assert((layout_data.child == UNSET_ID || layout_data.child == NO_ID || layout_data.child == id) && "Widget only allowed to have one child!");
        layout_data.child = id;
    }
    else if (l.type == Layout::MULTI_CHILD)
    {
        auto &layout_data = get_layout_data<MultiChildLayout>(l);
        layout_data.children.push_back(id);
    }
}

IMGUI::Transform IMGUI::get_child_transform(Layout &l, UI_ID child)
{
    assert(l.type != Layout::NO_CHILD && "Widget not allowed to have children!");
    if (l.type == Layout::SINGLE_CHILD)
    {
        auto &layout_data = get_layout_data<SingleChildLayout>(l);
        return layout_data.child_transform;
    }
    else if (l.type == Layout::MULTI_CHILD)
    {
        auto &layout_data = get_layout_data<MultiChildLayout>(l);
        if (layout_data.children_transforms.find(child) == layout_data.children_transforms.end())
            return layout_data.default_transform;
        return layout_data.children_transforms[child];
    }
    assert("how...");
    return Transform();
}

IMGUI::Constraints IMGUI::layout_get_constraints(Layout &l, UI_ID id, u32 index)
{
    assert(l.type != Layout::NO_CHILD && "Cannot get constraints from a no child widget parent!");
    if (l.type == Layout::SINGLE_CHILD)
    {
        const auto &layout_data = get_layout_data<SingleChildLayout>(l);
        return layout_data.child_constraints;
    }
    else if (l.type == Layout::MULTI_CHILD)
    {
        const auto &layout_data = get_layout_data<MultiChildLayout>(l);
        return layout_data.default_constraints;
    }
    assert(
        "???? How did you even get here you sad fuck??? How is that even physically possible. You had one job, pick one of 3 members of an enum, and you chose a fucking fourth option... how do you even do that???");
    return {};
}

template <>
IMGUI::NoChildLayout &IMGUI::get_layout_data(Layout &l)
{
    assert(l.type == Layout::NO_CHILD && "Layout is not a no child layout!");
    return l.data.no_child;
}

template <>
IMGUI::SingleChildLayout &IMGUI::get_layout_data(Layout &l)
{
    assert(l.type == Layout::SINGLE_CHILD && "Layout is not a single child layout!");
    return l.data.single_child;
}

template <>
IMGUI::MultiChildLayout &IMGUI::get_layout_data(Layout &l)
{
    assert(l.type == Layout::MULTI_CHILD && "Layout is not a multi-child layout!");
    return l.data.multi_child;
}

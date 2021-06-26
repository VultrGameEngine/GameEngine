#pragma once
#include <gui/core/ui_id.h>
#include "constraints.h"

namespace Vultr
{
    namespace IMGUI
    {
        // Context forward declaration
        struct Context;

        struct NoChildLayout
        {
            // I can't think of anything that the no child layout would need, but it's here just in case
        };

        struct SingleChildLayout
        {
            // The UI_ID of the child of this single child layout
            UI_ID child = UNSET_ID;

            // The constraints the child should receive when rendering/laying out
            Constraints child_constraints;

            // The position of the child that is calculated when the single child layout finishes
            Vec2 child_position = Vec2(0, 0);

            // Can render
            bool render_child = false;
        };

        struct MultiChildLayout
        {
            // The list of UI_IDs for the multi child widget
            std::vector<UI_ID> children;

            // The constraints to set for that widget in case they are not found in the child_constraints map.
            // This will occur when either the multi child widget renders for the first time (and as such cannot have had your desired size to set a constraint)
            // or when the child widgets of a multi child widget are changed between frames
            Constraints default_constraints;
            Vec2 default_position = Vec2(0, 0);

            // The saved constraints from the previous frame that will be applied to the child. NOTE: This member does not HAVE to be used
            std::unordered_map<UI_ID, Constraints> child_constraints;

            // The positions of each of the children calculated when the multi child layout finishes
            std::unordered_map<UI_ID, Vec2> children_positions;

            // Can render
            bool render_child = false;
        };

        union LayoutData {
            NoChildLayout no_child;
            SingleChildLayout single_child;
            MultiChildLayout multi_child;

            // Stupid cpp shit
            LayoutData()
            {
                memset(this, 0, sizeof(LayoutData));
            }
            ~LayoutData(){};
        };

        struct Layout
        {
            // The widget that this layout belongs to
            UI_ID owner = UNSET_ID;

            // The parent widget of the layout/the widget this layout belongs to
            UI_ID parent = NO_ID;

            // The type of layout
            enum Type
            {
                NO_CHILD,
                SINGLE_CHILD,
                MULTI_CHILD
            };

            // By default layouts are no_child
            Type type = NO_CHILD;

            // The size of the widget after constraints and whatnot are taken into account is stored here in the layout
            Vec2 local_size = Vec2(-1);

            // The data from here on differs based on type
            LayoutData data = LayoutData();

            // Stupid cpp shit
            Layout();
            ~Layout() = default;
            Layout(const Layout &other);
            void operator=(const Layout &other);
        };

        // Add a child widget to the layout
        void layout_add_child(Layout &l, UI_ID id);

        Vec2 get_child_position(Layout &l, UI_ID child);

        // Get the current constraints. This will change dependineg on the type of layout, however the basic idea is that the constraints the parent wants for the child will be
        // stored in the data section of its layout which the child will then (hopefully) abide by, creating a size in its own layout
        Constraints layout_get_constraints(Layout &l, UI_ID id, u32 index = 0);

        Layout new_no_child_layout(UI_ID owner, Vec2 local_size, UI_ID parent = NO_ID);
        Layout new_single_child_layout(UI_ID owner, Vec2 local_size, Constraints constraints, UI_ID parent = NO_ID);
        Layout new_multi_child_layout(UI_ID owner, Vec2 local_size, Constraints default_constraints, Vec2 default_position, UI_ID parent = NO_ID);

        // Get the layout data based on type safely using this. It is NOT recommended to do this through the layout directly, as that could very easily lead to bad data
        template <typename T>
        T &get_layout_data(Layout &l);

        // The actual implementations are explicitly instantiated
        template <>
        NoChildLayout &get_layout_data(Layout &l);

        template <>
        SingleChildLayout &get_layout_data(Layout &l);

        template <>
        MultiChildLayout &get_layout_data(Layout &l);

    } // namespace IMGUI
} // namespace Vultr

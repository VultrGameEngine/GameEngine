#pragma once
#include <gui/core/ui_id.h>

namespace Vultr
{
    namespace IMGUI
    {
        struct Size
        {
            enum SizingType
            {
                ABSOLUTE,
                RELATIVE_PARENT,
                RELATIVE_CHILD,
            };

            SizingType type;
            Vec2 values;
        };

        struct Layout
        {
            UI_ID parent;

            enum LayoutType
            {
                NO_CHILD,
                SINGLE_CHILD,
                MULTI_CHILD
            };

            LayoutType type = NO_CHILD;

            UI_ID child = UNSET_ID;
            std::set<UI_ID> children;
        };

        Vec2 get_absolute_size(Size size, Layout layout);

        void layout_add_child(UI_ID id);
        void layout_finalize(UI_ID id);
    } // namespace IMGUI
} // namespace Vultr

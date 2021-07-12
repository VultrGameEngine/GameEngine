#pragma once
#include <gui/core/context.h>
#include <gui/layout/alignment.h>
#include <gui/layout/edge_insets.h>
#include <gui/layout/constraints.h>

namespace Vultr
{
    namespace IMGUI
    {
        struct ContainerStyle
        {
            // The alignment of the child widget within the container
            Alignment child_alignment = Alignment::CENTER();

            // Color of the container
            Color color = Color(0);

            // Margin for the container
            EdgeInsets margin = EdgeInsets::ALL(0);

            // Padding within the container for the child
            EdgeInsets padding = EdgeInsets::ALL(0);

            // Additional constraints to be applied to the child
            Constraints constraints = Constraints();
        };

        struct ContainerState
        {
            ContainerStyle style = {};
        };

        // Draw a container without a child
        void container(Context *c, UI_ID id, ContainerStyle style = {});

        // Draw container with a single child
        void begin_container(Context *c, UI_ID id, ContainerStyle style = {});
        void end_container(Context *c);
    } // namespace IMGUI
} // namespace Vultr

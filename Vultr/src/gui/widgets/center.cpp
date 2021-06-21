#include <gui/widgets/center.h>

using namespace Vultr;

void IMGUI::begin_center(IMGUI::Context *c, UI_ID id)
{
    c->layout_stack.push({
        .parent = id,
    });
}

void IMGUI::end_center(IMGUI::Context *c)
{
    auto layout = end_layout(c);

    // Get child size
}

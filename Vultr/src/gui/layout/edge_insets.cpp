#include <gui/layout/edge_insets.h>

using namespace Vultr;

IMGUI::EdgeInsets IMGUI::edge_insets_symmetrical(f32 horizontal, f32 vertical)
{
    return {
        .left = horizontal,
        .right = horizontal,
        .top = vertical,
        .bottom = vertical,
    };
}
IMGUI::EdgeInsets IMGUI::edge_insets_all(f32 all)
{
    return {
        .left = all,
        .right = all,
        .top = all,
        .bottom = all,
    };
}

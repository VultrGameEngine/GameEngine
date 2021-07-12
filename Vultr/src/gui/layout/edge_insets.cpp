#include <gui/layout/edge_insets.h>

using namespace Vultr::IMGUI;

EdgeInsets EdgeInsets::SYMMETRICAL(f32 horizontal, f32 vertical)
{
    return {
        .left = horizontal,
        .right = horizontal,
        .top = vertical,
        .bottom = vertical,
    };
}

EdgeInsets EdgeInsets::ALL(f32 all)
{
    return {
        .left = all,
        .right = all,
        .top = all,
        .bottom = all,
    };
}

bool has_insets(const EdgeInsets &insets)
{
    return insets.top != 0 || insets.bottom != 0 || insets.left != 0 || insets.right != 0;
}

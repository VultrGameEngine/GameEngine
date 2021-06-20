#include <gui/widgets/rectangle.h>

using namespace Vultr;

void IMGUI::rect(Context *c, Color color, Vec2 size)
{
    draw_rect(c, color.value, Vec2(0, 0), size);
}

#include <gui/widgets/rounded_rect.h>
#include <gui/materials/default_rounded_material.h>

using namespace Vultr;

#define __rounded_rect_cache_id ui_id(__FILE__)

void IMGUI::rounded_rect(Context *c, UI_ID id, RoundedRectStyle style)
{
    Constraints parent_constraints = get_constraints(c, id);
    auto ncl = new_no_child_layout(__rounded_rect_cache_id, id, constraints_max(parent_constraints));
    layout_widget(c, id, ncl);

    auto *mat = new_rounded_material(c, style.color, Vec4(style.corner_radius));
    draw_rect(c, id, Vec2(0), ncl.local_size, mat);
}

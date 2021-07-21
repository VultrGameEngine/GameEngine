#include <gui/widgets/image.h>
#include <gui/materials/default_gui_material.h>

using namespace Vultr;

#define __image_cache_id ui_id(__FILE__)

template <>
void IMGUI::destroy_cache<IMGUI::ImageState>(Context *c, ImageState &cache){};

void IMGUI::image(Context *c, UI_ID id, Texture tex)
{
    auto &state = get_widget_cache<ImageState>(c, __image_cache_id, id);
    auto size = constraints_max(get_constraints(c, id));
    auto ncl = new_no_child_layout(__image_cache_id, id, size);
    layout_widget(c, id, ncl);
    draw_rect(c, id, Vec2(0), size, new_gui_material(c, Color(255), tex));
}

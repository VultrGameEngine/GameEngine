#include <gui/widgets/image.h>

using namespace Vultr;

#define __image_cache_id ui_id(__FILE__)

void IMGUI::image(Context *c, UI_ID id, Texture *tex, Vec2 position, Vec2 size)
{
    auto &state = get_widget_cache<ImageState>(c, __image_cache_id, id);
    draw_texture(c, tex, position, size);
    // return result;
}

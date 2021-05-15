// The Render System will loop through all entities with the renderer component
// The render component holds the ids for all of the buffers
// The render system will bind these ids and render them via opengl

#pragma once
#include <types/types.hpp>

struct UpdateTick;
struct ViewportData;

namespace Vultr::RenderSystem
{
    void register_system();
    void init();
    void update(const UpdateTick &meta_data);

    void init_g_buffer(int width, int height);
    void generate_render_texture(ViewportData &data, int width, int height);
    void resize(int width, int height, u8 type);
    void update_viewport_pos(int x, int y, u8 type);
} // namespace Vultr::RenderSystem

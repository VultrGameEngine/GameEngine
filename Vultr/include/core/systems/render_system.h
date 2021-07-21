// The Render System will loop through all entities with the renderer component
// The render component holds the ids for all of the buffers
// The render system will bind these ids and render them via opengl

#pragma once
#include <types/types.hpp>

namespace Vultr
{
    struct UpdateTick;
    struct ViewportData;
} // namespace Vultr

namespace Vultr::RenderSystem
{
    void register_system();
    void init();
    void update(const UpdateTick &meta_data);

    void init_g_buffer(s32 width, s32 height);
    void resize(s32 width, s32 height, u8 type);
    void update_viewport_pos(s32 x, s32 y, u8 type);
} // namespace Vultr::RenderSystem

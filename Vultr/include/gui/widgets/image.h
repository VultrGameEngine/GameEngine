#pragma once
#include <gui/core/context.h>
#include <rendering/models/texture.h>

namespace Vultr
{
    namespace IMGUI
    {
        struct ImageState
        {
        };

        void image(Context *c, UI_ID id, Texture *tex, Vec2 size, Vec2 position);

    } // namespace IMGUI
} // namespace Vultr

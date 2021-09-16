#pragma once
#include <gui/core/context.h>
#include <rendering/types/texture.h>

namespace Vultr
{
    namespace IMGUI
    {
        struct ImageState
        {
        };

        void image(Context *c, UI_ID id, Texture *tex);

    } // namespace IMGUI
} // namespace Vultr

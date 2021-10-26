#pragma once
#include <engine.hpp>
#include <types/types.h>

namespace Vultr
{
    namespace IMGUI
    {
        struct Window
        {
            Vec2 dimensions;
            Vec2 position;
        };

        Window new_window(Engine *e);
    } // namespace IMGUI
} // namespace Vultr

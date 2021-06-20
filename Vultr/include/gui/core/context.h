#pragma once
#include "window.h"
#include <gui/rendering/renderer.h>
#include <stack>

namespace Vultr
{
    namespace IMGUI
    {
        struct Context
        {
            Window window;

            Vec2 cursor_position;
            Vec2 cusor_dimension;

            Renderer renderer;
        };

        Context *new_context(const Window &window);

        void begin(Context *c);
        void end(Context *c);

        bool mouse_over(Vec2 top_left, Vec2 size);

        void draw_rect(Context *c, Vec4 color, Vec2 position, Vec2 dimensions, Shader *shader = nullptr);

    } // namespace IMGUI
} // namespace Vultr

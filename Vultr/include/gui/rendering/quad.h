#pragma once
#include "gui_vertex.h"
#include <core/models/color.h>

namespace Vultr
{
    namespace IMGUI
    {
        struct Context;
        struct Quad
        {
            GUIVertex vertices[4];
            Quad()
            {
                vertices[0].position = Vec3(-1, -1, 0); // bottom left
                vertices[1].position = Vec3(-1, 1, 0);  // top left
                vertices[2].position = Vec3(1, -1, 0);  // bottom right
                vertices[3].position = Vec3(1, 1, 0);   // top right

                vertices[0].uv = Vec2(0, 0);
                vertices[1].uv = Vec2(0, 1);
                vertices[2].uv = Vec2(1, 0);
                vertices[3].uv = Vec2(1, 1);
            }
        };

        void set_quad_transform(Context *c, Quad &quad, Vec2 position, Vec2 scale);
        void set_quad_texture_slot(Context *c, Quad &quad, f32 texture_slot);
        void set_quad_color(Context *c, Quad &quad, Color color);
    } // namespace IMGUI
} // namespace Vultr

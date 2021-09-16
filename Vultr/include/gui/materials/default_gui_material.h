#pragma once
#include <gui/rendering/material.h>
#include <gui/core/context.h>
#include <core/models/color.h>

namespace Vultr
{
    namespace IMGUI
    {
        struct DefaultGUIMaterial : Material
        {
            ~DefaultGUIMaterial() = default;

            void bind() override;

            Color color;
            Texture *texture;
        };

        DefaultGUIMaterial *new_gui_material(Context *c, Color color, Texture *texture = nullptr);
    } // namespace IMGUI
} // namespace Vultr

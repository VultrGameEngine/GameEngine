#pragma once
#include <gui/rendering/material.h>
#include <gui/core/context.h>
#include <core/models/color.h>

namespace Vultr
{
    namespace IMGUI
    {
        struct DefaultRoundedMaterial : Material
        {
            ~DefaultRoundedMaterial() = default;

            void bind() override;

            Color color;
            Vec4 corner_radius;
        };

        DefaultRoundedMaterial *new_rounded_material(Context *c, Color color, Vec4 corner_radius);
    } // namespace IMGUI
} // namespace Vultr

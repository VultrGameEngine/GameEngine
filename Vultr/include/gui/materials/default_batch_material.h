#pragma once
#include <gui/rendering/material.h>
#include <gui/core/context.h>

namespace Vultr
{
    namespace IMGUI
    {
        struct DefaultBatchMaterial : Material
        {
            ~DefaultBatchMaterial() = default;

            void bind() override;

            Texture *texture;
            s32 samplers[16];
        };
        DefaultBatchMaterial *new_batch_material(Context *c, Texture *texture);
    } // namespace IMGUI
} // namespace Vultr

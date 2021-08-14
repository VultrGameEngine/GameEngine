#pragma once
#include <core/components/material_component.h>

namespace Vultr
{
    namespace UnlitMaterial
    {
#define UNLIT_MATERIAL_SOURCE ShaderSource("shaders/unlit.glsl")
        MaterialComponent Create(const Color &color, f32 emission);

    } // namespace UnlitMaterial
} // namespace Vultr

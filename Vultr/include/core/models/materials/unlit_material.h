#pragma once
#include <core/components/material_component.h>

namespace Vultr
{
    namespace UnlitMaterial
    {
#define UNLIT_MATERIAL_SOURCE "shaders/unlit.glsl"
        MaterialComponent Create();

    } // namespace UnlitMaterial
} // namespace Vultr

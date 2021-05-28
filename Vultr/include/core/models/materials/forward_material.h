#pragma once
#include <core/components/material_component.h>

namespace Vultr
{
    namespace ForwardMaterial
    {
#define FORWARD_MATERIAL_SOURCE "shaders/forward_material.glsl"
        MaterialComponent Create(const char *p_texture);
    } // namespace ForwardMaterial
} // namespace Vultr

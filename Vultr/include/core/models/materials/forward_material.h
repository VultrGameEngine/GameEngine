#pragma once
#include <core/components/material_component.h>

namespace Vultr
{
#define FORWARD_MATERIAL_SOURCE ShaderSource("shaders/forward_material.glsl")
    namespace ForwardMaterial
    {
        MaterialComponent Create(const char *diffuse, const char *specular);
    }

} // namespace Vultr

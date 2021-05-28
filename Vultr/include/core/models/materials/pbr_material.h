#pragma once
#include <core/components/material_component.h>

namespace Vultr
{
    namespace PBRMaterial
    {
#define PBR_MATERIAL_SOURCE "shaders/forward_material.glsl"
        MaterialComponent Create(const std::string &p_diffuse_path, const std::string &p_specular_path);

    } // namespace PBRMaterial

} // namespace Vultr

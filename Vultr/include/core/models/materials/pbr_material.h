#pragma once
#include <core/components/material_component.h>

namespace Vultr
{
    namespace PBRMaterial
    {
#define PBR_MATERIAL_SOURCE ShaderSource("shaders/pbr.glsl")

        struct PBRMaterial
        {
            const char *albedo_map = "";
            Vec4 albedo = Vec4(1);

            const char *normal_map = "";

            const char *metallic_map = "";
            float metallic = 0;

            const char *roughness_map = "";
            float roughness = 0.5;
            bool is_smoothness_map = false;

            const char *ambient_occlusion_map = "";
            float ambient_occlusion = 0;
        };

        MaterialComponent Create(const PBRMaterial &material);

    } // namespace PBRMaterial

} // namespace Vultr

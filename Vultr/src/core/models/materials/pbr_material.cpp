#include <core/models/materials/pbr_material.h>

namespace Vultr
{
    namespace PBRMaterial
    {
        MaterialComponent Create(const PBRMaterial &material)
        {
            MaterialComponent component = MaterialComponent();
            component.shader_source = PBR_MATERIAL_SOURCE;
            texture_uniform(component, "u_Albedo_map", TextureSource(material.albedo_map), "u_Has_albedo_map");
            color_uniform(component, "u_Albedo", material.albedo);

            texture_uniform(component, "u_Normal_map", TextureSource(material.normal_map), "u_Has_normal_map");

            texture_uniform(component, "u_Metallic_map", TextureSource(material.metallic_map), "u_Has_metallic_map");
            f32_uniform(component, "u_Metallic", material.metallic);

            texture_uniform(component, "u_Roughness_map", TextureSource(material.roughness_map), "u_Has_roughness_map");
            f32_uniform(component, "u_Roughness", material.roughness);

            bool_uniform(component, "u_Is_smoothness_map", material.is_smoothness_map);

            texture_uniform(component, "u_Ambient_occlusion_map", TextureSource(material.ambient_occlusion_map), "u_Has_ambient_occlusion_map");
            f32_uniform(component, "u_Ambient_occlusion", material.ambient_occlusion);

            return component;
        }

    }; // namespace PBRMaterial

} // namespace Vultr

#include <core/models/materials/forward_material.h>
#include <rendering/types/shader.h>

namespace Vultr
{
    namespace ForwardMaterial
    {
        MaterialComponent Create(const char *diffuse, const char *specular, const char *normal)
        {
            MaterialComponent component = MaterialComponent();
            component.shader_source = FORWARD_MATERIAL_SOURCE;
            texture_uniform(component, "u_Diffuse", TextureSource(diffuse));
            texture_uniform(component, "u_Specular", TextureSource(specular), "u_Has_specular_map");
            texture_uniform(component, "u_Normal", TextureSource(normal), "u_Has_normal_map");
            color_uniform(component, "u_Tint", Color(255));
            f32_uniform(component, "u_Shininess", 1.0);
            return component;
        }
    } // namespace ForwardMaterial
} // namespace Vultr

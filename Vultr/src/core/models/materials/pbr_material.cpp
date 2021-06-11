#include <core/models/materials/pbr_material.h>

namespace Vultr
{
    namespace PBRMaterial
    {
        MaterialComponent Create(const char *p_diffuse_path, const char *p_specular_path)
        {
            MaterialComponent component = MaterialComponent();
            component.shader_source = ShaderSource(PBR_MATERIAL_SOURCE);
            component.textures.push_back({TextureSource(p_diffuse_path), 0, "Diffuse"});
            component.textures.push_back({TextureSource(p_specular_path), 1, "Specular"});
            return component;
        }

    }; // namespace PBRMaterial

} // namespace Vultr

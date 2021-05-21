#include <core/models/materials/pbr_material.h>

namespace Vultr
{
    namespace PBRMaterial
    {
        MaterialComponent Create(const char *p_diffuse_path, const char *p_specular_path)
        {
            MaterialComponent component = MaterialComponent();
            component.shader_path = "shaders/material.glsl";
            component.textures.push_back({File(p_diffuse_path, TEXTURE_FILE_EXTENSIONS), 0, "Diffuse"});
            component.textures.push_back({File(p_specular_path, TEXTURE_FILE_EXTENSIONS), 1, "Specular"});
            return component;
        }

    }; // namespace PBRMaterial

} // namespace Vultr

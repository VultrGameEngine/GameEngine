#include <core/models/materials/pbr_material.h>

namespace Vultr
{
namespace PBRMaterial
{
MaterialComponent Create(const std::string &p_diffuse_path,
                         const std::string &p_specular_path)
{
    MaterialComponent component = MaterialComponent();
    component.shader_path = "res/shaders/material.glsl";
    component.textures.push_back({p_diffuse_path, 0});
    component.textures.push_back({p_specular_path, 1});
    return component;
}

}; // namespace PBRMaterial

} // namespace Vultr

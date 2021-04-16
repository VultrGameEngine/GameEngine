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
    component.textures.insert({p_diffuse_path, GL_TEXTURE0});
    component.textures.insert({p_specular_path, GL_TEXTURE1});
    return component;
}

}; // namespace PBRMaterial

} // namespace Vultr

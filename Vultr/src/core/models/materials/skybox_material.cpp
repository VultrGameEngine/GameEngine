#include <core/models/materials/skybox_material.h>

namespace Vultr
{
namespace SkyboxMaterial
{
MaterialComponent Create(std::string p_identifier = "default")
{
    MaterialComponent component = MaterialComponent();
    component.shader_path =
        "/home/brandon/Dev/GameEngine/SandboxTesting/res/shaders/skybox.glsl";
    component.identifier = &p_identifier[0];
    component.textures.insert({p_identifier, GL_TEXTURE0});
    component.ints.insert({"skybox", 0});
    return component;
}

} // namespace SkyboxMaterial
} // namespace Vultr

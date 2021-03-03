#include <core/models/materials/forward_material.h>

namespace Vultr
{
namespace ForwardMaterial
{
MaterialComponent Create(const std::string &p_texture =
                             "/home/brandon/Dev/GameEngine/SandboxTesting/"
                             "res/textures/clone/albedo.jpeg")
{
    MaterialComponent component = MaterialComponent();
    component.shader_path =
        "/home/brandon/Dev/GameEngine/SandboxTesting/res/shaders/"
        "forward_material.glsl";
    component.textures.insert({p_texture, GL_TEXTURE0});
    component.vec3s.insert({"objectColor", glm::vec3(1, 1, 1)});
    component.vec3s.insert({"lightColor", glm::vec3(1, 1, 1)});
    component.ints.insert({"textureSampler", 0});
    return component;
}
} // namespace ForwardMaterial
} // namespace Vultr

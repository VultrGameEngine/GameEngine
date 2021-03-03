#include <core/models/materials/unlit_material.h>

namespace Vultr
{
namespace UnlitMaterial
{
MaterialComponent Create()
{
    MaterialComponent component = MaterialComponent();
    component.shader_path =
        "/home/brandon/Dev/GameEngine/SandboxTesting/res/shaders/unlit.glsl";
    component.vec3s.insert({"lightColor", glm::vec3(1, 1, 1)});
    return component;
}

} // namespace UnlitMaterial
} // namespace Vultr

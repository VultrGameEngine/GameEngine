#include <core/models/materials/unlit_material.h>

namespace Vultr
{
namespace UnlitMaterial
{
MaterialComponent Create()
{
    MaterialComponent component = MaterialComponent();
    component.shader_path = "res/shaders/unlit.glsl";
    component.colors.insert({"lightColor", Color(glm::vec4(1))});
    return component;
}

} // namespace UnlitMaterial
} // namespace Vultr

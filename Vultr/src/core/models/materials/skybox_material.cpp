#include <core/models/materials/skybox_material.h>

namespace Vultr
{
    namespace SkyboxMaterial
    {
        MaterialComponent Create(const char *p_identifier = "default")
        {
            MaterialComponent component = MaterialComponent();
            component.shader_path = "res/shaders/skybox.glsl";
            component.identifier = p_identifier;
            component.textures.push_back({p_identifier, 0});
            component.ints.insert({"skybox", 0});
            return component;
        }

    } // namespace SkyboxMaterial
} // namespace Vultr

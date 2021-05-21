#include <core/models/materials/forward_material.h>

namespace Vultr
{
    namespace ForwardMaterial
    {
        MaterialComponent Create(const char *p_texture = "textures/clone/albedo.jpeg")
        {
            MaterialComponent component = MaterialComponent();
            component.shader_path = "shaders/forward_material.glsl";
            component.textures.push_back({File(p_texture, TEXTURE_FILE_EXTENSIONS), 0, "Albedo"});
            component.colors.insert({"objectColor", Color(glm::vec4(1))});
            component.colors.insert({"lightColor", Color(glm::vec4(1))});
            component.ints.insert({"textureSampler", 0});
            return component;
        }
    } // namespace ForwardMaterial
} // namespace Vultr

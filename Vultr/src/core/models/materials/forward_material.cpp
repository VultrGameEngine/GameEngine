#include <core/models/materials/forward_material.h>

namespace Vultr
{
    namespace ForwardMaterial
    {
        MaterialComponent Create(const char *p_texture = "textures/clone/albedo.jpeg")
        {
            MaterialComponent component = MaterialComponent();
            component.shader_source = File("shaders/forward_material.glsl", SHADER_FILE_EXTENSIONS);
            component.textures.push_back({File(p_texture, TEXTURE_FILE_EXTENSIONS), 0, "Diffuse"});
            component.textures.push_back({File(p_texture, TEXTURE_FILE_EXTENSIONS), 1, "Specular"});
            component.colors.insert({"tint", Color(glm::vec4(1))});
            component.ints.insert({"material.diffuse", 0});
            component.ints.insert({"material.specular", 1});
            component.floats.insert({"material.shininess", 1.0});
            return component;
        }
    } // namespace ForwardMaterial
} // namespace Vultr

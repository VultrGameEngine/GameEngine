#include <core/models/materials/skybox_material.h>

namespace Vultr
{
    namespace SkyboxMaterial
    {
        MaterialComponent Create(const char *p_identifier = "default")
        {
            MaterialComponent component = MaterialComponent();
            component.shader_source = File("shaders/skybox.glsl", SHADER_FILE_EXTENSIONS);
            component.identifier = p_identifier;
            // TODO fix this
            component.textures.push_back({File(p_identifier, TEXTURE_FILE_EXTENSIONS), 0, "Identifier"});
            component.ints.insert({"skybox", 0});
            return component;
        }

    } // namespace SkyboxMaterial
} // namespace Vultr

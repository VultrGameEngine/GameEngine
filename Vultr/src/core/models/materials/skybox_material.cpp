#include <core/models/materials/skybox_material.h>

namespace Vultr
{
    namespace SkyboxMaterial
    {
        MaterialComponent Create(const char *front, const char *back, const char *top, const char *bottom, const char *left, const char *right)
        {
            MaterialComponent component = MaterialComponent();
            component.shader_source = File("shaders/skybox.glsl", SHADER_FILE_EXTENSIONS);
            // TODO fix this
            component.textures.push_back({File(right, TEXTURE_FILE_EXTENSIONS), 0, "Right"});
            component.textures.push_back({File(left, TEXTURE_FILE_EXTENSIONS), 0, "Left"});
            component.textures.push_back({File(top, TEXTURE_FILE_EXTENSIONS), 0, "Top"});
            component.textures.push_back({File(bottom, TEXTURE_FILE_EXTENSIONS), 0, "Bottom"});
            component.textures.push_back({File(front, TEXTURE_FILE_EXTENSIONS), 0, "Front"});
            component.textures.push_back({File(back, TEXTURE_FILE_EXTENSIONS), 0, "Back"});
            component.ints.insert({"skybox", 0});
            return component;
        }

    } // namespace SkyboxMaterial
} // namespace Vultr

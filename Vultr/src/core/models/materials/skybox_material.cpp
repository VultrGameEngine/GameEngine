#include <core/models/materials/skybox_material.h>

namespace Vultr
{
    namespace SkyboxMaterial
    {
        MaterialComponent Create(const char *front, const char *back, const char *top, const char *bottom, const char *left, const char *right)
        {
            MaterialComponent component = MaterialComponent();
            component.shader_source = ShaderSource(SKYBOX_MATERIAL_SOURCE);
            component.textures.push_back({TextureSource(right), 0, "Right"});
            component.textures.push_back({TextureSource(left), 0, "Left"});
            component.textures.push_back({TextureSource(top), 0, "Top"});
            component.textures.push_back({TextureSource(bottom), 0, "Bottom"});
            component.textures.push_back({TextureSource(front), 0, "Front"});
            component.textures.push_back({TextureSource(back), 0, "Back"});
            component.ints.insert({"skybox", 0});
            return component;
        }

    } // namespace SkyboxMaterial
} // namespace Vultr

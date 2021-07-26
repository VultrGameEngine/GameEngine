#include <core/models/materials/skybox_material.h>

namespace Vultr
{
    namespace SkyboxMaterial
    {
        MaterialComponent Create(const char *front, const char *back, const char *top, const char *bottom, const char *left, const char *right)
        {
            MaterialComponent component = MaterialComponent();
            component.shader_source = SKYBOX_MATERIAL_SOURCE;
            // component.textures[0] = {TextureSource(right), 0, "Right"};
            // component.textures[1] = {TextureSource(left), 0, "Left"};
            // component.textures[2] = {TextureSource(top), 0, "Top"};
            // component.textures[3] = {TextureSource(bottom), 0, "Bottom"};
            // component.textures[4] = {TextureSource(front), 0, "Front"};
            // component.textures[5] = {TextureSource(back), 0, "Back"};
            // component.material.uniforms[2] = s32_uniform(0);
            return component;
        }

    } // namespace SkyboxMaterial
} // namespace Vultr

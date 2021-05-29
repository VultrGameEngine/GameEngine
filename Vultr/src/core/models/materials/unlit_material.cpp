#include <core/models/materials/unlit_material.h>

namespace Vultr
{
    namespace UnlitMaterial
    {
        MaterialComponent Create()
        {
            MaterialComponent component = MaterialComponent();
            component.shader_source = ShaderSource(UNLIT_MATERIAL_SOURCE);
            component.colors.insert({"lightColor", Color(Vec4(1))});
            return component;
        }

    } // namespace UnlitMaterial
} // namespace Vultr

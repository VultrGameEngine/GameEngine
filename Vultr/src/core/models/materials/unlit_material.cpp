#include <core/models/materials/unlit_material.h>

namespace Vultr
{
    namespace UnlitMaterial
    {
        MaterialComponent Create(const Color &color, f32 emission)
        {
            MaterialComponent component = MaterialComponent();
            component.shader_source = UNLIT_MATERIAL_SOURCE;
            color_uniform(component, "u_Color", color);
            f32_uniform(component, "u_Emission", emission);
            return component;
        }

    } // namespace UnlitMaterial
} // namespace Vultr

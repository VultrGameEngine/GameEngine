#pragma once
#include <types/types.hpp>
#include <core/models/color.h>

namespace Vultr
{
    struct LightComponent
    {
        const static u8 DirectionalLight = 0;
        const static u8 PointLight = 1;
        const static u8 SpotLight = 2;
        static LightComponent Create()
        {
            LightComponent component = LightComponent();
            return component;
        }

        u8 type = DirectionalLight;
        f32 intensity = 1.0;

        // Universal
        Color ambient = Color();
        Color diffuse = Color();
        f32 specular = 0.5;

        // Point
        f32 constant = 0;
        f32 linear = 0;
        f32 quadratic = 0;

        // Spot
        // TBD
    };
} // namespace Vultr

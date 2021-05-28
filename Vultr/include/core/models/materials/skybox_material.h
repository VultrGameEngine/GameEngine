#pragma once
#include <core/components/material_component.h>

namespace Vultr
{
    namespace SkyboxMaterial
    {
#define SKYBOX_MATERIAL_SOURCE "shaders/skybox.glsl"
        MaterialComponent Create(const char *front, const char *back, const char *top, const char *bottom, const char *left, const char *right);

    } // namespace SkyboxMaterial
} // namespace Vultr

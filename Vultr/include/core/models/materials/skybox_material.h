#pragma once
#include <core/components/material_component.h>

namespace Vultr
{
    namespace SkyboxMaterial
    {
        MaterialComponent Create(const char *front, const char *back, const char *top, const char *bottom, const char *left, const char *right);

    } // namespace SkyboxMaterial
} // namespace Vultr

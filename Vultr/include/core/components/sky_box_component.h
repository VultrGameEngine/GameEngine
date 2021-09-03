#pragma once
#include <stdlib.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>
#include <types/types.hpp>

namespace Vultr
{
    struct SkyBoxComponent
    {
        static SkyBoxComponent Create()
        {
            SkyBoxComponent component = SkyBoxComponent();
            component.identifier = 1; // rand() % 10000;
            return component;
        }

        u32 identifier;
    };
} // namespace Vultr

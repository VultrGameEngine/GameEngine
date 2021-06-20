#pragma once
#include "material.h"
#include <rendering/models/shader.h>

namespace Vultr
{
    namespace IMGUI
    {
        struct RenderRequest
        {
            Material *material;
            glm::mat4 transform;
        };
    } // namespace IMGUI
} // namespace Vultr

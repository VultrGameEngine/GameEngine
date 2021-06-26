#pragma once
#include <rendering/models/shader.h>

namespace Vultr
{
    namespace IMGUI
    {
        struct Material
        {
            Shader *shader;
            virtual void bind() = 0;
            virtual ~Material() = default;
        };
    } // namespace IMGUI
} // namespace Vultr

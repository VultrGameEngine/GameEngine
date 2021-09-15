#pragma once
#include <types/types.hpp>

namespace Vultr
{
    struct InternalShader
    {
        u32 id = 0;
    };

#define invalid_shader()                                                                                                                                                                                              \
    InternalShader                                                                                                                                                                                                    \
    {                                                                                                                                                                                                                 \
    }
} // namespace Vultr

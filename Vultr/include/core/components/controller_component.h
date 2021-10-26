#pragma once
#include <types/types.h>

namespace Vultr
{
    struct ControllerComponent
    {
        static ControllerComponent Create()
        {
            ControllerComponent component = ControllerComponent();
            return component;
        }
        f32 sens = 0.03;
    };
} // namespace Vultr

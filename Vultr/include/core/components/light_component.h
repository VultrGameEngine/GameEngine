#pragma once
#include <types/types.hpp>

struct LightComponent
{
    static LightComponent Create()
    {
        LightComponent component = LightComponent();
        return component;
    }
    s32 some_param = 0;
};

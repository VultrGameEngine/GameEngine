#pragma once
#include <types/types.hpp>

struct ControllerComponent
{
    static ControllerComponent Create()
    {
        ControllerComponent component = ControllerComponent();
        return component;
    }
    f32 sens = 0.03;
};

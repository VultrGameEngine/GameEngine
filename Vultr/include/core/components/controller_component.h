#pragma once

struct ControllerComponent
{
    static ControllerComponent Create()
    {
        ControllerComponent component = ControllerComponent();
        return component;
    }
    float sens = 0.03;

    template <class Archive> void serialize(Archive &ar)
    {
        ar(sens);
    }
};

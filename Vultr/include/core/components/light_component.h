#pragma once

struct LightComponent
{
    static LightComponent Create()
    {
        LightComponent component = LightComponent();
        return component;
    }
    int some_param = 0;

    template <class Archive> void serialize(Archive &ar)
    {
        ar(some_param);
    }
};

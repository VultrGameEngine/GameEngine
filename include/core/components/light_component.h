#pragma once

struct LightComponent
{
    static LightComponent &Create()
    {
        LightComponent *component = new LightComponent();
        return *component;
    }
};

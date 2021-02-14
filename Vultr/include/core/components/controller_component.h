#pragma once
struct ControllerComponent
{
    static ControllerComponent &Create()
    {
        ControllerComponent *component = new ControllerComponent();
        return *component;
    }
    float sens = 0.03;
};

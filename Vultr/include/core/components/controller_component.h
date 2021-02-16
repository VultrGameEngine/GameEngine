#pragma once
#include <memory>

struct ControllerComponent
{
    static std::shared_ptr<ControllerComponent> Create()
    {
        std::shared_ptr<ControllerComponent> component =
            std::make_shared<ControllerComponent>();
        return component;
    }
    float sens = 0.03;
};

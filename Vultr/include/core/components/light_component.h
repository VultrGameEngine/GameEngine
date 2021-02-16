#pragma once
#include <memory>

struct LightComponent
{
    static std::shared_ptr<LightComponent> Create()
    {
        std::shared_ptr<LightComponent> component =
            std::make_shared<LightComponent>();
        return component;
    }
};

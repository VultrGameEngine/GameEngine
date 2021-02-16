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
    int some_param = 0;

    template <class Archive> void serialize(Archive &ar)
    {
        ar(some_param);
    }
};

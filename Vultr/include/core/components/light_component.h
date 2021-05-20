#pragma once
#include <json/json.hpp>

struct LightComponent
{
    static LightComponent Create()
    {
        LightComponent component = LightComponent();
        return component;
    }
    int some_param = 0;

    template <class Archive>
    void serialize(Archive &ar)
    {
        ar(some_param);
    }
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(LightComponent, some_param);

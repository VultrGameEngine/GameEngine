#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>
#include <helpers/file.h>
#include <types/types.hpp>
#include <json/json.hpp>
#include <json/glm_serializer.hpp>

struct SkyBoxComponent
{
    static SkyBoxComponent Create()
    {
        static s16 identifier = 0;
        SkyBoxComponent component = SkyBoxComponent();
        component.identifier = std::to_string(++identifier);
        return component;
    }

    std::string identifier;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SkyBoxComponent, identifier);

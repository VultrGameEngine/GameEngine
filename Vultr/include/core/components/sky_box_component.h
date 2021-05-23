#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>
#include <helpers/file.h>
#include <types/types.hpp>

struct SkyBoxComponent
{
    static SkyBoxComponent Create()
    {
        // TODO Fix this because this will not work
        static s16 identifier = 0;
        SkyBoxComponent component = SkyBoxComponent();
        component.identifier = std::to_string(++identifier);
        return component;
    }

    std::string identifier;
};

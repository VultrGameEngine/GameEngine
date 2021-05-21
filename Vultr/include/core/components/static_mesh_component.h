#pragma once
#include <glm/glm.hpp>
#include <rendering/models/mesh.h>
#include <string>
#include <json/json.hpp>

struct StaticMeshComponent
{
    StaticMeshComponent()
    {
    }

    static StaticMeshComponent Create(const std::string &p_path = "models/cube.obj")
    {
        StaticMeshComponent component = StaticMeshComponent();
        component.path = p_path;
        return component;
    }

    std::string path;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(StaticMeshComponent, path);

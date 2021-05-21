#pragma once
#include <glm/glm.hpp>
#include <rendering/models/mesh.h>
#include <string>
#include <helpers/file.h>
#include <json/json.hpp>

struct StaticMeshComponent
{
    StaticMeshComponent()
    {
    }

    static StaticMeshComponent Create(const std::string &p_path = "models/cube.obj")
    {
        StaticMeshComponent component = StaticMeshComponent();
        component.source = Vultr::File(p_path, MODEL_3D_FILE_EXTENSIONS);
        return component;
    }

    Vultr::File source;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(StaticMeshComponent, source);

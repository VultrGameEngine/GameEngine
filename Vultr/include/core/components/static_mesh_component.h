#pragma once
#include <glm/glm.hpp>
#include <rendering/models/mesh.h>
#include <string>
#include <helpers/file.h>

struct StaticMeshComponent
{
    StaticMeshComponent()
    {
    }

    static StaticMeshComponent Create(const std::string &p_path = "models/cube.obj")
    {
        StaticMeshComponent component = StaticMeshComponent();
        component.source = Vultr::ModelSource(p_path);
        return component;
    }

    Vultr::ModelSource source;
};

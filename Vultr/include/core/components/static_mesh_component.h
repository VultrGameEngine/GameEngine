#pragma once
#include <glm/glm.hpp>
#include <rendering/models/mesh.h>
#include <string>

struct StaticMeshComponent
{
    StaticMeshComponent()
    {
    }

    static StaticMeshComponent Create(const std::string &p_path = "res/models/cube.obj")
    {
        StaticMeshComponent component = StaticMeshComponent();
        component.path = p_path;
        return component;
    }

    std::string path;
};

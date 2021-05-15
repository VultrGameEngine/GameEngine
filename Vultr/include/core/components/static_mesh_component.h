#pragma once
#include <glm/glm.hpp>
#include <rendering/models/mesh.h>
#include <string>
#include <vector>

struct StaticMeshComponent
{
    StaticMeshComponent()
    {
    }

    static StaticMeshComponent Create(const char *p_path = "res/models/cube.obj")
    {
        StaticMeshComponent component = StaticMeshComponent();
        component.path = p_path;
        return component;
    }

    const char *path;
};

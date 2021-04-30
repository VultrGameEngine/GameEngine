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

    static StaticMeshComponent Create(std::string p_path = "res/models/cube.obj")
    {
        StaticMeshComponent component = StaticMeshComponent();
        component.m_path = p_path;
        return component;
    }

    std::string m_path;
};

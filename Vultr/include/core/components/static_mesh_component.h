#pragma once
#include <core/system_providers/mesh_loader_system_provider.h>
#include <glm/glm.hpp>
#include <rendering/models/mesh.h>
#include <string>
#include <vector>

struct StaticMeshComponent
{
    StaticMeshComponent()
    {
    }
    template <class Archive> void serialize(Archive &ar)
    {
        ar(m_path);
    }

    static StaticMeshComponent Create(std::string p_path = "res/models/cube.obj")
    {
        StaticMeshComponent component = StaticMeshComponent();
        component.m_path = p_path;
        return component;
    }

    std::string m_path;

    Vultr::Mesh *GetMesh()
    {
        // We cannot find a mesh for a path that has not been intialized
        if (m_path.empty())
            return nullptr;
        return Vultr::MeshLoaderSystemProvider::GetMesh(m_path);
    }
};

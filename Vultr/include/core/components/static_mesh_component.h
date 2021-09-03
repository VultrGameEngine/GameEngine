#pragma once
#include <glm/glm.hpp>
#include <rendering/types/mesh.h>
#include <string>
#include <filesystem/file.h>

namespace Vultr
{

    struct StaticMeshComponent
    {
        static StaticMeshComponent Create(const char *path = "models/cube.obj")
        {
            StaticMeshComponent component = StaticMeshComponent();
            component.source = Vultr::ModelSource(path);
            return component;
        }

        Vultr::ModelSource source;
    };
} // namespace Vultr

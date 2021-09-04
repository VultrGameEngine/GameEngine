#pragma once
#include <map>
#include <rendering/types/mesh.h>
#include <string>
#include <filesystem/file.h>

namespace Vultr::MeshImporter
{
    Mesh import_mesh(const ModelSource &source);
    Mesh init_quad();
    Mesh init_skybox();

} // namespace Vultr::MeshImporter

#pragma once
#include <map>
#include <rendering/models/mesh.h>
#include <string>

namespace Vultr
{
    class MeshImporter
    {
      public:
        static Mesh *ImportMesh(const std::string &path);
        static Mesh *InitQuad();
        static Mesh *InitSkybox();
    };

} // namespace Vultr

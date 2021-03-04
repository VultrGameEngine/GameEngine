#pragma once
#include <map>
#include <rendering/models/mesh.h>
#include <string>

namespace Vultr
{
class MeshImporter
{
  public:
    static Mesh *ImportMesh(std::string path);
    static Mesh *InitQuad();
};

} // namespace Vultr

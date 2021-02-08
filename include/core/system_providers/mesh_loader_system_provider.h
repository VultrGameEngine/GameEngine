#pragma once
#include <ecs/system/system_provider.hpp>
#include <rendering/models/mesh.h>
#include <unordered_map>

namespace Brick3D
{
class MeshLoaderSystemProvider : public SystemProvider
{
  public:
    static MeshLoaderSystemProvider &Get()
    {
        static MeshLoaderSystemProvider instance;
        return instance;
    }
    Signature signature;

    static Mesh *GetMesh(std::string path)
    {
        // If we do not have that mesh, return nullptr
        if (Get().meshes.find(path) == Get().meshes.end())
        {
            return nullptr;
        }

        return Get().meshes[path];
    }

    static void AddMesh(std::string path, Mesh *mesh)
    {
        Get().meshes[path] = mesh;
    }

  private:
    std::unordered_map<std::string, Mesh *> meshes;
};

} // namespace Brick3D

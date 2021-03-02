// MeshLoaderSystem loads in all the specified meshes in all of the entities
// that have a StaticMeshComponent If there are entities that have a
// StaticMeshComponent and do not have their buffers or opengl data set then
// MeshLoaderSystem will do that with the specified path

#pragma once
#include <core/system_providers/mesh_loader_system_provider.h>

namespace Vultr
{

class MeshLoaderSystem
{
  public:
    static void RegisterSystem();
    static void Update();
    static void OnCreateEntity(Entity entity);
    static SystemProvider &GetProvider()
    {
        return *MeshLoaderSystemProvider::Get();
    }

  protected:
    static void CheckAndLoadMesh(Entity entity);

  private:
    static void Import(std::string path);
};
} // namespace Vultr

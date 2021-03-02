#pragma once
#include <ecs/system/system_provider.hpp>
#include <rendering/models/mesh.h>

namespace Vultr
{
class MeshLoaderSystemProvider : public SystemProvider
{
  public:
    static std::shared_ptr<MeshLoaderSystemProvider> Get();

    static Mesh *GetMesh(std::string path)
    {
        // If we do not have that mesh, return nullptr
        if (Get()->meshes.find(path) == Get()->meshes.end())
        {
            return nullptr;
        }

        return Get()->meshes[path];
    }

    static void AddMesh(std::string path, Mesh *mesh)
    {
        Get()->meshes[path] = mesh;
    }

  protected:
    void OnDestroyEntity(Entity entity) override;
    void OnCreateEntity(Entity entity) override;

  private:
    std::unordered_map<std::string, Mesh *> meshes;
};

} // namespace Vultr

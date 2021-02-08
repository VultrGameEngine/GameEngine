// MeshLoaderSystem loads in all the specified meshes in all of the entities
// that have a StaticMeshComponent If there are entities that have a
// StaticMeshComponent and do not have their buffers or opengl data set then
// MeshLoaderSystem will do that with the specified path

#pragma once
#include <core/components/static_mesh_component.h>
#include <core/system_providers/mesh_loader_system_provider.h>
#include <ecs/system/system.hpp>
#include <ecs/world/world.hpp>
#include <memory>

namespace Brick3D
{

class MeshLoaderSystem : public System
{
  public:
    static void RegisterSystem();

  protected:
    void OnCreateEntity(Entity entity) override;
    SystemProvider &GetProvider() override
    {
        return MeshLoaderSystemProvider::Get();
    }

  private:
    static void Import(std::string path);
};
} // namespace Brick3D

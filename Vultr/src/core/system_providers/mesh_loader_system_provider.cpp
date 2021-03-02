#include <core/system_providers/mesh_loader_system_provider.h>
#include <core/systems/mesh_loader_system.h>
#include <ecs/world/world.hpp>

namespace Vultr
{
std::shared_ptr<MeshLoaderSystemProvider> MeshLoaderSystemProvider::Get()
{
    return World::GetSystemProvider<MeshLoaderSystemProvider>();
}

void MeshLoaderSystemProvider::OnCreateEntity(Entity entity)
{
    MeshLoaderSystem::OnCreateEntity(entity);
}

void MeshLoaderSystemProvider::OnDestroyEntity(Entity entity)
{
}
} // namespace Vultr

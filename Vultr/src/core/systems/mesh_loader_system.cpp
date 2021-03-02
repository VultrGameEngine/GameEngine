#include <core/systems/mesh_loader_system.h>
#include <fstream>
#include <helpers/mesh_importer.h>
#include <iostream>
#include <map>
#include <sstream>
#include <string.h>
#include <string>
#include <thread>
#include <vector>
#include <ecs/world/world.hpp>

namespace Vultr
{
void MeshLoaderSystem::RegisterSystem()
{
    MeshLoaderSystemProvider &provider = *MeshLoaderSystemProvider::Get();
    provider.signature.set(World::GetComponentType<StaticMeshComponent>());
    World::RegisterSystem<MeshLoaderSystemProvider>(provider.signature);
}

void MeshLoaderSystem::Update()
{
    MeshLoaderSystemProvider &provider = *MeshLoaderSystemProvider::Get();
    for (Entity entity : provider.entities)
    {
        CheckAndLoadMesh(entity);
    }
}

void MeshLoaderSystem::OnCreateEntity(Entity entity)
{
    CheckAndLoadMesh(entity);
}

void MeshLoaderSystem::CheckAndLoadMesh(Entity entity)
{
    auto &component = World::GetComponent<StaticMeshComponent>(entity);

    if (component.GetMesh() == nullptr)
    {
        Import(component.m_path);
    }
}

void MeshLoaderSystem::Import(std::string path)
{
    MeshLoaderSystemProvider &provider = *MeshLoaderSystemProvider::Get();
    Mesh *mesh = MeshImporter::ImportMesh(path);
    provider.AddMesh(path, mesh);
}

} // namespace Vultr

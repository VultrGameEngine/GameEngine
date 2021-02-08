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

namespace Brick3D
{
void MeshLoaderSystem::RegisterSystem()
{
    MeshLoaderSystemProvider &provider = MeshLoaderSystemProvider::Get();
    provider.signature.set(World::GetComponentType<StaticMeshComponent>());
    std::shared_ptr<MeshLoaderSystem> ptr =
        World::RegisterSystem<MeshLoaderSystem>(provider.signature);
}

void MeshLoaderSystem::OnCreateEntity(Entity entity)
{
    auto &component = World::GetComponent<StaticMeshComponent>(entity);

    if (component.GetMesh() == nullptr)
    {
        std::thread loadingThread(Import, component.m_path);
        loadingThread.detach();
    }
}

void MeshLoaderSystem::Import(std::string path)
{
    MeshLoaderSystemProvider &provider = MeshLoaderSystemProvider::Get();
    Mesh *mesh = MeshImporter::ImportMesh(path);
    provider.AddMesh(path, mesh);
}

} // namespace Brick3D

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
#include <engine.hpp>

namespace Vultr
{
void MeshLoaderSystem::RegisterSystem()
{
    Signature signature;
    signature.set(
        Engine::GetComponentRegistry().GetComponentType<StaticMeshComponent>());
    Engine::RegisterGlobalSystem<MeshLoaderSystemProvider>(signature);
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
    auto &component = entity.GetComponent<StaticMeshComponent>();

    if (component.GetMesh() == nullptr)
    {
        Import(component.m_path);
    }
}

void MeshLoaderSystem::Import(std::string path)
{
    MeshLoaderSystemProvider &provider = *MeshLoaderSystemProvider::Get();
    Mesh *mesh = MeshImporter::ImportMesh(path);
    if (mesh == nullptr)
        return;
    provider.AddMesh(path, mesh);
}

} // namespace Vultr

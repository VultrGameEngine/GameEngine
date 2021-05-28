#include <core/system_providers/mesh_loader_system_provider.h>
#include <core/systems/mesh_loader_system.h>
#include <ecs/world/world.hpp>
#include <engine.hpp>

namespace Vultr::MeshLoaderSystem
{
    Component &get_provider()
    {
        return *get_global_system_provider<Component>();
    }

    Mesh *get_mesh(const ModelSource &source)
    {
        auto &p = get_provider();
        // If we do not have that mesh, return nullptr
        if (p.meshes.find(source.path.string()) == p.meshes.end())
        {
            return nullptr;
        }

        return p.meshes[source.path.string()];
    }

    void add_mesh(const ModelSource &source, Mesh *mesh)
    {
        get_provider().meshes[source.path.string()] = mesh;
    }
} // namespace Vultr::MeshLoaderSystem

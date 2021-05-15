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

    Mesh *get_mesh(const char *path)
    {
        auto &p = get_provider();
        // If we do not have that mesh, return nullptr
        if (p.meshes.find(path) == p.meshes.end())
        {
            return nullptr;
        }

        return p.meshes[path];
    }

    void add_mesh(const char *path, Mesh *mesh)
    {
        get_provider().meshes[path] = mesh;
    }
} // namespace Vultr::MeshLoaderSystem

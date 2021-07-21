#include <core/system_providers/mesh_loader_system_provider.h>
#include <core/systems/mesh_loader_system.h>
#include <ecs/world/world.hpp>
#include <engine.hpp>

namespace Vultr::MeshLoaderSystem
{
    Component &get_provider(Engine *e)
    {
        return *get_global_system_provider<Component>(e);
    }

    Mesh get_mesh(Engine *e, const ModelSource &source)
    {
        auto &p = get_provider(e);
        // If we do not have that mesh, return nullptr
        if (p.meshes.find(source.path.string()) == p.meshes.end())
        {
            return {};
        }

        return p.meshes[source.path.string()];
    }

    void add_mesh(Engine *e, const ModelSource &source, Mesh mesh)
    {
        get_provider(e).meshes[source.path.string()] = mesh;
    }
} // namespace Vultr::MeshLoaderSystem

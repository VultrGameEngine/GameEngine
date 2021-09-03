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

    // TODO: Get rid of this std string shit
    Mesh get_mesh(Engine *e, const ModelSource &source)
    {
        auto &p = get_provider(e);
        // If we do not have that mesh, return nullptr
        if (p.meshes.find(std::string(source.path)) == p.meshes.end())
        {
            return {};
        }

        return p.meshes[std::string(source.path)];
    }

    void add_mesh(Engine *e, const ModelSource &source, Mesh mesh)
    {
        get_provider(e).meshes[std::string(source.path)] = mesh;
    }
} // namespace Vultr::MeshLoaderSystem

#include <core/systems/mesh_loader_system.h>
#include <core/system_providers/mesh_loader_system_provider.h>
#include <fstream>
#include <helpers/mesh_importer.h>
#include <iostream>
#include <map>
#include <sstream>
#include <string.h>
#include <string>
#include <thread>
#include <ecs/world/world.hpp>
#include <engine.hpp>

namespace Vultr::MeshLoaderSystem
{
    static void import(Engine *e, const ModelSource &source)
    {
        auto &provider = get_provider(e);
        Mesh mesh = MeshImporter::import_mesh(source);
        if (!is_valid_mesh(mesh))
            return;
        add_mesh(e, source, mesh);
    }
    static void check_and_load_mesh(Engine *e, Entity entity)
    {
        auto &component = entity_get_component<StaticMeshComponent>(e, entity);
        auto source = component.source;

        auto mesh = get_mesh(e, source);
        if (!is_valid_mesh(mesh))
        {
            MeshLoaderSystem::import(e, source);
        }
    }
    void register_system(Engine *e)
    {
        Signature signature;
        signature.set(get_component_type<StaticMeshComponent>(e), true);
        register_global_system<Component>(e, signature, on_create_entity, nullptr);
    }

    void update(Engine *e)
    {
        auto &p = get_provider(e);
        for (Entity entity : p.entities)
        {
            check_and_load_mesh(e, entity);
        }
    }

    void on_create_entity(Engine *e, Entity entity)
    {
        check_and_load_mesh(e, entity);
    }

} // namespace Vultr::MeshLoaderSystem

#include <core/components/material_component.h>
#include <core/system_providers/shader_loader_system_provider.h>
#include <core/systems/shader_loader_system.h>
#include <ecs/component/component.hpp>
#include <helpers/shader_importer.h>
#include <engine.hpp>

namespace Vultr::ShaderLoaderSystem
{
    static void check_and_load_shader(Entity entity);
    void register_system()
    {
        Signature signature;
        signature.set(get_component_type<MaterialComponent>(), true);
        register_global_system<Component>(signature, on_create_entity, on_destroy_entity);
    }

    void update()
    {
        auto &provider = get_provider();
        for (Entity entity : provider.entities)
        {
            check_and_load_shader(entity);
        }
    }

    void on_create_entity(Entity entity)
    {
        check_and_load_shader(entity);
    }

    void on_destroy_entity(Entity entity)
    {
        // auto &shader_component = World::GetComponent<MaterialComponent>(entity);
        // Shader *shader = shader_component.GetShader();
        // if (shader != nullptr)
        // {
        //     delete shader;
        // }
    }

    void load_shader(const MaterialComponent &mat)
    {
        auto &provider = get_provider();
        // Get the material path
        auto path = mat.shader_source.path.string();

        // If we have already loaded the shader and cached it, then reuse the id and
        // don't reload
        auto *material_shader = get_shader(path.c_str());
        if (material_shader != nullptr)
            return;

        // If we haven't cached this shader, load it and save it in the loaded shaders
        // Create the shader on the gpu
        Shader *shader = ShaderImporter::ImportShader(path);

        // Create the shader wrapper with the given shader id
        // Shader *shader = new Shader(shader_id, Forward);

        // Add it to the loaded shaders
        provider.loaded_shaders[path] = shader;
    }

    static void check_and_load_shader(Entity entity)
    {
        auto &material_component = entity_get_component<MaterialComponent>(entity);
        load_shader(material_component);
    }

} // namespace Vultr::ShaderLoaderSystem

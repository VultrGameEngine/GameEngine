#include <core/components/material_component.h>
#include <core/system_providers/shader_loader_system_provider.h>
#include <core/systems/shader_loader_system.h>
#include <ecs/component/component.hpp>
#include <helpers/shader_importer.h>
#include <engine.hpp>

namespace Vultr::ShaderLoaderSystem
{
    static void check_and_load_shader(Engine *e, Entity entity);

    void register_system(Engine *e)
    {
        Signature signature;
        signature.set(get_component_type<MaterialComponent>(e), true);
        register_global_system<Component>(e, signature, on_create_entity, on_destroy_entity);
    }

    void update(Engine *e)
    {
        auto &provider = get_provider(e);
        for (Entity entity : provider.entities)
        {
            check_and_load_shader(e, entity);
        }
    }

    void on_create_entity(Engine *e, Entity entity)
    {
        check_and_load_shader(e, entity);
    }

    void on_destroy_entity(Engine *e, Entity entity)
    {
        // auto &shader_component = World::GetComponent<MaterialComponent>(entity);
        // Shader *shader = shader_component.GetShader();
        // if (shader != nullptr)
        // {
        //     delete shader;
        // }
    }

    void load_shader(Engine *e, const MaterialComponent &mat)
    {
        auto &provider = get_provider(e);
        // Get the material path
        auto source = mat.shader_source;

        // If we have already loaded the shader and cached it, then reuse the id and
        // don't reload
        auto material_shader = get_shader(e, source);
        if (is_valid_shader(material_shader))
            return;

        // If we haven't cached this shader, load it and save it in the loaded shaders
        // Create the shader on the gpu
        Shader shader = ShaderImporter::import_shader(source);

        // Attach all of our uniform buffers to the shader
        for (u16 i = 0; i < UNIFORM_BUFFER_COUNT; i++)
        {
            auto &ubo = provider.ubos[i];
            attach_shader_uniform_buffer(shader, ubo);
        }

        // Add it to the loaded shaders
        provider.loaded_shaders[std::string(source.path)] = shader;
    }

    static void check_and_load_shader(Engine *e, Entity entity)
    {
        auto &material_component = entity_get_component<MaterialComponent>(e, entity);
        load_shader(e, material_component);
    }

} // namespace Vultr::ShaderLoaderSystem

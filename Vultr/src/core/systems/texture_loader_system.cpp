#include <glad/glad.h>
#include <core/components/material_component.h>
#include <core/system_providers/texture_loader_system_provider.h>
#include <core/systems/texture_loader_system.h>
#include <helpers/texture_importer.h>
#include <stb_image/stb_image.h>
#include <engine.hpp>

namespace Vultr::TextureLoaderSystem
{
    void check_and_load_texture(Engine *e, Entity entity);

    void register_system(Engine *e)
    {
        Signature signature;
        signature.set(get_component_type<MaterialComponent>(e));
        register_global_system<Component>(e, signature, on_create_entity, nullptr);
    }

    void update(Engine *e)
    {
        auto &provider = get_provider(e);
        for (Entity entity : provider.entities)
        {
            check_and_load_texture(e, entity);
        }
    }

    void on_create_entity(Engine *e, Entity entity)
    {
        check_and_load_texture(e, entity);
    }

    void load_texture(Engine *e, const MaterialComponent &mat)
    {
        auto &provider = get_provider(e);
        for (auto [source, name, optional_location] : mat.textures)
        {
            if (strlen(name) == 0)
                continue;
            if (!is_loaded(e, source))
            {
                provider.textures[std::string(source.path)] = {};
                TextureImporter::import(provider.textures[std::string(source.path)], source);
            }
        }
    }

    void check_and_load_texture(Engine *e, Entity entity)
    {
        auto &provider = get_provider(e);
        auto &component = entity_get_component<MaterialComponent>(e, entity);
        Signature signature;
        signature.set(get_component_type<SkyBoxComponent>(e));
        if (entity_has_component<SkyBoxComponent>(e, entity))
        {
            auto &skybox_component = entity_get_component<SkyBoxComponent>(e, entity);
            // TODO: Make this hashed by default
            // auto skybox_id = std::to_string(skybox_component.identifier);
            // if (!is_loaded(skybox_id))
            // {
            //     auto texture = generate_texture(GL_TEXTURE_CUBE_MAP);
            //     provider.textures[skybox_id] = texture;
            //     TextureImporter::import_skybox(texture, component.get_paths());
            // }
        }
        else
        {
            for (auto [file, name, optional_location] : component.textures)
            {
                if (strlen(name) == 0)
                    continue;
                if (!is_loaded(e, file))
                {
                    Texture new_tex;
                    generate_texture(&new_tex, GL_TEXTURE_2D);
                    bool successful = TextureImporter::import(new_tex, file);
                    if (successful)
                    {
                        provider.textures[std::string(file.path)] = new_tex;
                    }
                    else
                    {
                        if (strequal(optional_location, ""))
                        {
                            fprintf(stderr, "Failed to import texture %s!", file.path);
                        }
                        delete_texture(&new_tex);
                    }
                }
            }
        }
    }

} // namespace Vultr::TextureLoaderSystem

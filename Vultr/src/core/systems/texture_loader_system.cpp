#include <glad/glad.h>
#include <core/components/material_component.h>
#include <core/system_providers/texture_loader_system_provider.h>
#include <core/systems/texture_loader_system.h>
#include <helpers/texture_importer.h>
#include <stb_image/stb_image.h>
#include <engine.hpp>

namespace Vultr::TextureLoaderSystem
{
    void check_and_load_texture(Entity entity);

    void register_system()
    {
        Signature signature;
        signature.set(get_component_type<MaterialComponent>());
        register_global_system<Component>(signature, on_create_entity, nullptr);
    }

    void update()
    {
        auto &provider = get_provider();
        for (Entity entity : provider.entities)
        {
            check_and_load_texture(entity);
        }
    }

    void on_create_entity(Entity entity)
    {
        check_and_load_texture(entity);
    }

    void load_texture(const MaterialComponent &mat)
    {
        auto &provider = get_provider();
        for (auto [source, slot, name] : mat.textures)
        {
            if (!is_loaded(source))
            {
                provider.textures[source.path.string()] = new Texture(GL_TEXTURE_2D);
                TextureImporter::import(source, *provider.textures[source.path.string()]);
            }
        }
    }

    void check_and_load_texture(Entity entity)
    {
        auto &provider = get_provider();
        auto &component = entity_get_component<MaterialComponent>(entity);
        Signature signature;
        signature.set(get_component_type<SkyBoxComponent>());
        if (entity_has_component<SkyBoxComponent>(entity))
        {
            auto &skybox_component = entity_get_component<SkyBoxComponent>(entity);
            if (!is_loaded(skybox_component.identifier.c_str()))
            {
                Texture *texture = new Texture(GL_TEXTURE_CUBE_MAP);
                provider.textures[skybox_component.identifier] = texture;
                TextureImporter::import_skybox(component.get_paths(), *texture);
            }
        }
        else
        {
            for (auto [file, slot, name] : component.textures)
            {
                if (!is_loaded(file))
                {
                    Texture *new_tex = new Texture(GL_TEXTURE_2D);
                    bool successful = TextureImporter::import(file, *new_tex);
                    if (successful)
                    {
                        provider.textures[file.path.string()] = new_tex;
                    }
                    else
                    {
                        delete new_tex;
                    }
                }
            }
        }
    }

} // namespace Vultr::TextureLoaderSystem

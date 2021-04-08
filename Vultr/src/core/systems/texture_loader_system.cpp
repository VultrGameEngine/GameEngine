#include <glad/glad.h>
#include <core/components/material_component.h>
#include <core/system_providers/texture_loader_system_provider.h>
#include <core/systems/texture_loader_system.h>
#include <ecs/world/world.hpp>
#include <fstream>
#include <helpers/loading_state.h>
#include <helpers/texture_importer.h>
#include <iostream>
#include <map>
#include <sstream>
#include <stb_image/stb_image.h>
#include <string.h>
#include <string>
#include <thread>
#include <vector>
#include <engine.hpp>

namespace Vultr
{

void TextureLoaderSystem::RegisterSystem()
{
    Signature signature;
    signature.set(
        Engine::GetComponentRegistry().GetComponentType<MaterialComponent>());
    Engine::RegisterGlobalSystem<TextureLoaderSystemProvider>(signature);
}

void TextureLoaderSystem::Update()
{
    TextureLoaderSystemProvider &provider = *TextureLoaderSystemProvider::Get();
    for (Entity entity : provider.entities)
    {
        CheckAndLoadTexture(entity);
    }
}

void TextureLoaderSystem::OnCreateEntity(Entity entity)
{
    CheckAndLoadTexture(entity);
}

void TextureLoaderSystem::CheckAndLoadTexture(Entity entity)
{
    TextureLoaderSystemProvider &provider = *TextureLoaderSystemProvider::Get();
    auto &component = entity.GetComponent<MaterialComponent>();
    if (component.identifier != nullptr)
    {
        auto &skybox_component = entity.GetComponent<SkyBoxComponent>();
        if (!provider.isLoaded(skybox_component.identifier))
        {
            Texture *texture = new Texture(GL_TEXTURE_CUBE_MAP);
            provider.textures[skybox_component.identifier] = texture;
            TextureImporter::ImportSkybox(skybox_component.GetPaths(), *texture);
        }
    }
    else
    {
        for (auto [path, slot] : component.textures)
        {
            if (!provider.isLoaded(path))
            {
                provider.textures[path] = new Texture(GL_TEXTURE_2D);
                TextureImporter::Import(path, *provider.textures[path]);
            }
        }
    }
}

} // namespace Vultr

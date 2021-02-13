#include <GL/glew.h>
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

namespace Brick3D
{

void TextureLoaderSystem::RegisterSystem()
{
    TextureLoaderSystemProvider &provider = TextureLoaderSystemProvider::Get();
    provider.signature.set(World::GetComponentType<MaterialComponent>());
    World::RegisterSystem<TextureLoaderSystem>(provider.signature);
}

void TextureLoaderSystem::Update()
{
    TextureLoaderSystemProvider &provider = TextureLoaderSystemProvider::Get();
    for (Entity entity : provider.entities)
    {
        auto &component = World::GetComponent<MaterialComponent>(entity);
        for (std::string path : component.GetTextures())
        {
            if (!provider.isLoaded(path))
            {
                provider.textures[path] = new Texture(GL_TEXTURE_2D);
                TextureImporter::Import(path, *provider.textures[path]);
            }
        }
    }
    // Signature skybox_signature;
    // skybox_signature.set(World::GetComponentType<SkyBoxComponent>());
    // for (Entity entity : World::GetEntities(skybox_signature))
    // {
    //     auto &component = World::GetComponent<SkyBoxComponent>(entity);
    //     if (!isLoaded(component.identifier))
    //     {
    //         state.textures[component.identifier] = {};
    //         ImportSkybox(component.GetPaths(),
    //         state.textures[component.identifier]);
    //     }
    // }
}

void TextureLoaderSystem::OnCreateEntity(Entity entity)
{
}

} // namespace Brick3D

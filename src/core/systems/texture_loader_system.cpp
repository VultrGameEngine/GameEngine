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

void TextureLoaderSystem::OnCreateEntity(Entity entity)
{
    TextureLoaderSystemProvider &provider = TextureLoaderSystemProvider::Get();
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

} // namespace Brick3D

#include "../../../include/core/systems/texture_loader_system.h"
#include "../../../include/ecs/coordinator/coordinator.hpp"
#include "../../../include/helpers/loading_state.h"
#include "../../../vendor/stb_image/stb_image.h"
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string.h>
#include <thread>
#include "GLFW/glfw3.h"

std::shared_ptr<TextureLoaderSystem> TextureLoaderSystem::instance = nullptr;
TextureCache TextureLoaderSystem::state = {};

std::shared_ptr<TextureLoaderSystem> TextureLoaderSystem::Get()
{
    if (instance == 0)
    {

        instance = RegisterSystem();
    }
    return instance;
}

std::shared_ptr<TextureLoaderSystem> TextureLoaderSystem::RegisterSystem()
{
    std::shared_ptr<TextureLoaderSystem> ptr = Coordinator::Get()->RegisterSystem<TextureLoaderSystem>();
    return ptr;
}
void TextureLoaderSystem::DestroyEntity(Entity entity)
{
}

void TextureLoaderSystem::Update()
{
    Signature signature;
    signature.set(Coordinator::Get()->GetComponentType<TextureComponent>());
    for (Entity entity : Coordinator::Get()->GetEntities(signature))
    {
        auto &component = Coordinator::Get()->GetComponent<TextureComponent>(entity);
        if (!isLoaded(component.path))
        {
            state.textures[component.path] = {};
            Import(component.path, state.textures[component.path]);
        }
    }
}
LoadedTexture *TextureLoaderSystem::GetTexture(std::string mesh)
{
    if (isLoaded(mesh))
    {
        return &(state.textures[mesh]);
    }
    else
    {
        return nullptr;
    }
}

bool TextureLoaderSystem::isLoaded(std::string texture)
{
    return state.textures.find(texture) != state.textures.end();
}

void TextureLoaderSystem::Import(std::string path, LoadedTexture &texture)
{
    texture.loaded = loading;
    stbi_set_flip_vertically_on_load(1);
    texture.buffer = stbi_load(path.c_str(), &texture.width, &texture.height, &texture.bpp, 4);
    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.buffer);
    glBindTexture(GL_TEXTURE_2D, 0);
    if (texture.buffer)
        stbi_image_free(texture.buffer);

    texture.loaded = loaded;
}
#include "../../../include/core/systems/texture_loader_system.h"
#include "../../../include/ecs/coordinator/coordinator.hpp"
#include "../../../include/helpers/loading_state.h"
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string.h>
#include <thread>

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
            // std::thread loadingThread(Import, component.path, std::ref(state.meshes[component.path]));
            // loadingThread.detach();
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


#pragma once
#include <unordered_map>
#include "../../helpers/loading_state.h"

struct LoadedTexture
{
    LoadingState loaded = notLoaded;
    int width;
    int height;
};

struct TextureCache
{
    std::unordered_map<std::string, LoadedTexture> textures;
};
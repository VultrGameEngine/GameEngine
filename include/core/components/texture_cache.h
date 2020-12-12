#pragma once
#include <unordered_map>
#include "../../helpers/loading_state.h"

struct LoadedTexture
{
    unsigned char *buffer;
    unsigned int id = 0;
    LoadingState loaded = notLoaded;
    int width;
    int height;
    int bpp;
};

struct TextureCache
{
    std::unordered_map<std::string, LoadedTexture> textures;
};
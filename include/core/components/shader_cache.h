#pragma once
#include <string>
#include <GL/glew.h>
#include <unordered_map>
#include "../../helpers/loading_state.h"

struct LoadedShader
{
    unsigned int id = 0;
    LoadingState loaded = notLoaded;
};

struct ShaderCache
{
    std::unordered_map<std::string, LoadedShader> textures;
};
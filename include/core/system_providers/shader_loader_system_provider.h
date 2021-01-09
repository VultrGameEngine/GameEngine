#pragma once
#include <ecs/system/system_provider.hpp>
#include <rendering/models/shader.h>
#include <unordered_map>

namespace Brick3D
{
class ShaderLoaderSystemProvider : public SystemProvider
{
  public:
    static ShaderLoaderSystemProvider &Get()
    {
        static ShaderLoaderSystemProvider instance;
        return instance;
    }
    static Shader *GetShader(std::string path)
    {
        if (Get().loaded_shaders.find(path) == Get().loaded_shaders.end())
            return nullptr;

        return Get().loaded_shaders[path];
    }

    Signature signature;
    std::unordered_map<std::string, Shader *> loaded_shaders;
};
} // namespace Brick3D

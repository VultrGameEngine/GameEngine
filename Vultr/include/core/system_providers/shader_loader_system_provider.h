#pragma once
#include <ecs/system/system_provider.hpp>
#include <rendering/models/shader.h>
#include <unordered_map>

namespace Vultr
{
class ShaderLoaderSystemProvider : public SystemProvider
{
  public:
    static std::shared_ptr<ShaderLoaderSystemProvider> Get();
    static Shader *GetShader(const std::string &path)
    {
        if (Get()->loaded_shaders.find(path) == Get()->loaded_shaders.end())
            return nullptr;

        return Get()->loaded_shaders[path];
    }

    std::unordered_map<std::string, Shader *> loaded_shaders;

    template <class Archive> void serialize(Archive &archive)
    {
        archive(); // Nothing needs to be serialized here
    }

  protected:
    void OnCreateEntity(Entity entity) override;
    void OnDestroyEntity(Entity entity) override;
};
} // namespace Vultr
VultrRegisterSystemProvider(Vultr::ShaderLoaderSystemProvider)

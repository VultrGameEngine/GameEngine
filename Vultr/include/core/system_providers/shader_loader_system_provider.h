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

    template <class Archive> void serialize(Archive &ar)
    {
        // We pass this cast to the base type for each base type we
        // need to serialize.  Do this instead of calling serialize functions
        // directly
        ar(cereal::base_class<SystemProvider>(this));
    }

  protected:
    void OnCreateEntity(Entity entity) override;
    void OnDestroyEntity(Entity entity) override;
};
} // namespace Vultr
VultrRegisterSystemProvider(Vultr::ShaderLoaderSystemProvider)

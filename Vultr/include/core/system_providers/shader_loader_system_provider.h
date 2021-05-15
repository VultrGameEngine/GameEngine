#pragma once
#include <ecs/system/system_provider.hpp>
#include <unordered_map>
#include <type_info/type_info.h>
#include <rendering/models/shader.h>

namespace Vultr
{

    namespace ShaderLoaderSystem
    {
        struct Component : public SystemProvider
        {
            std::unordered_map<std::string, Shader *> loaded_shaders;
        };

        Component &get_provider();

        Shader *get_shader(const char *path);
    } // namespace ShaderLoaderSystem

    template <>
    inline const char *get_struct_name<ShaderLoaderSystem::Component>()
    {
        return "ShaderLoaderSystem";
    }
} // namespace Vultr

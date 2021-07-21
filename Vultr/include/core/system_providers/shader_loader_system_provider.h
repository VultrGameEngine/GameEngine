#pragma once
#include <engine.hpp>
#include <ecs/system/system_provider.hpp>
#include <unordered_map>
#include <type_info/type_info.h>
#include <rendering/types/shader.h>
#include <helpers/file.h>

namespace Vultr
{

    namespace ShaderLoaderSystem
    {
        struct Component : public SystemProvider
        {
            std::unordered_map<std::string, Shader> loaded_shaders;
        };

        Component &get_provider(Engine *e);

        Shader get_shader(Engine *e, const ShaderSource &source);
    } // namespace ShaderLoaderSystem

    template <>
    inline const char *get_struct_name<ShaderLoaderSystem::Component>()
    {
        return "ShaderLoaderSystem";
    }
} // namespace Vultr

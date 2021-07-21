#include <core/system_providers/shader_loader_system_provider.h>
#include <core/systems/shader_loader_system.h>
#include <rendering/types/shader.h>
#include <engine.hpp>

namespace Vultr::ShaderLoaderSystem
{
    Component &get_provider(Engine *e)
    {
        return *get_global_system_provider<Component>(e);
    }

    Shader get_shader(Engine *e, const ShaderSource &source)
    {
        auto &p = get_provider(e);
        if (p.loaded_shaders.find(source.path.string()) == p.loaded_shaders.end())
            return invalid_shader();

        return p.loaded_shaders[source.path.string()];
    }
} // namespace Vultr::ShaderLoaderSystem

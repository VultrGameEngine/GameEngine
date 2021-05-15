#include <core/system_providers/shader_loader_system_provider.h>
#include <core/systems/shader_loader_system.h>
#include <rendering/models/shader.h>
#include <engine.hpp>

namespace Vultr::ShaderLoaderSystem
{
    Component &get_provider()
    {
        return *get_global_system_provider<Component>();
    }

    Shader *get_shader(const char *path)
    {
        auto &p = get_provider();
        if (p.loaded_shaders.find(path) == p.loaded_shaders.end())
            return nullptr;

        return p.loaded_shaders[path];
    }
} // namespace Vultr::ShaderLoaderSystem

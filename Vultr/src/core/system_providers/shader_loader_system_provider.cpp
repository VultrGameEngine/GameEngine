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

    void bind_camera_uniform(Engine *e)
    {
        auto &p = get_provider(e);
        bind_uniform_buffer(p.ubos[0]);
    }

    void set_camera_uniform(Engine *e, const CameraUniform &uniform)
    {
        auto &p = get_provider(e);

        auto &ubo = p.ubos[0];
        bind_uniform_buffer(ubo);

        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Vec4), &uniform.position.x);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Vec4), sizeof(Mat4), glm::value_ptr(uniform.view_matrix));
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Vec4) + sizeof(Mat4), sizeof(Mat4), glm::value_ptr(uniform.projection_matrix));
    }
} // namespace Vultr::ShaderLoaderSystem

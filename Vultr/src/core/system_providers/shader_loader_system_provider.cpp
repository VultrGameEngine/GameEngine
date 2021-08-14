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
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Vec4) + sizeof(Mat4) + sizeof(Mat4), sizeof(f32), &uniform.bloom_threshold);
    }

    void bind_directional_light_uniform(Engine *e)
    {
        auto &p = get_provider(e);
        bind_uniform_buffer(p.ubos[1]);
    }

    void set_directional_light_uniform(Engine *e, const DirectionalLightUniform &uniform)
    {
        auto &p = get_provider(e);

        auto &ubo = p.ubos[1];
        bind_uniform_buffer(ubo);

        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Vec4), &uniform.direction.x);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Vec4), sizeof(Vec4), &uniform.ambient.x);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Vec4) * 2, sizeof(Vec4), &uniform.diffuse.x);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Vec4) * 3, sizeof(f32), &uniform.specular);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Vec4) * 3 + sizeof(f32), sizeof(f32), &uniform.intensity);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Vec4) * 3 + sizeof(f32) * 2, sizeof(u16), &uniform.exists);
    }

    void bind_point_lights_uniform(Engine *e)
    {
        auto &p = get_provider(e);
        bind_uniform_buffer(p.ubos[2]);
    }

    void set_point_lights_uniform(Engine *e, const PointLightsUniform &uniform)
    {
        auto &p = get_provider(e);

        auto &ubo = p.ubos[2];
        bind_uniform_buffer(ubo);

        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Vec4) * MAX_POINT_LIGHTS, &uniform.positions[0].x);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Vec4) * 1 * MAX_POINT_LIGHTS, sizeof(Vec4) * MAX_POINT_LIGHTS, uniform.constants);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Vec4) * 2 * MAX_POINT_LIGHTS, sizeof(Vec4) * MAX_POINT_LIGHTS, uniform.linears);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Vec4) * 3 * MAX_POINT_LIGHTS, sizeof(Vec4) * MAX_POINT_LIGHTS, uniform.quadratics);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Vec4) * 4 * MAX_POINT_LIGHTS, sizeof(Vec4) * MAX_POINT_LIGHTS, &uniform.ambients[0].x);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Vec4) * 5 * MAX_POINT_LIGHTS, sizeof(Vec4) * MAX_POINT_LIGHTS, &uniform.diffuses[0].x);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Vec4) * 6 * MAX_POINT_LIGHTS, sizeof(Vec4) * MAX_POINT_LIGHTS, uniform.speculars);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Vec4) * 7 * MAX_POINT_LIGHTS, sizeof(Vec4) * MAX_POINT_LIGHTS, uniform.intensities);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Vec4) * 8 * MAX_POINT_LIGHTS, sizeof(u16), &uniform.count);
    }
} // namespace Vultr::ShaderLoaderSystem

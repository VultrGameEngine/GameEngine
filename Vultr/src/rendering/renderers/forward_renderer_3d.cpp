#include <rendering/renderers/forward_renderer_3d.h>
#include <core/system_providers/shader_loader_system_provider.h>
#include <core/system_providers/texture_loader_system_provider.h>
#include <core/system_providers/light_system_provider.h>
#include <glm/gtc/type_ptr.hpp>
#include <rendering/render_context.h>

namespace Vultr::Renderer3D
{
    void ForwardRenderer::Submit(const MaterialComponent &material, const Mat4 &transform, const Mesh &mesh, const char *skybox_identifier)
    {
        BindMaterial(material, transform, skybox_identifier);
        draw_mesh(mesh);
    }
    void ForwardRenderer::BindMaterial(const MaterialComponent &material, const Mat4 &transform, const char *skybox_identifier)
    {
        Shader shader = ShaderLoaderSystem::get_shader(material.shader_source);
        if (!is_valid_shader(shader))
            return;
        bind_shader(shader);
        glm::mat4 model = transform;
        set_uniform_matrix_4fv(shader, "model", glm::value_ptr(model));

        const RenderContext &context = RenderContext::GetContext();

        // shader->SetUniformMatrix4fv("projection", glm::value_ptr());
        glm::mat4 projection = context.camera_component.GetProjectionMatrix(context.dimensions.x, context.dimensions.y);

        Entity directional_light = LightSystem::get_provider().directional_light;
        if (directional_light != INVALID_ENTITY)
        {
            auto &light_component = entity_get_component<LightComponent>(directional_light);
            auto &transform_component = entity_get_component<TransformComponent>(directional_light);
            set_uniform_3f(shader, "directional_light.direction", -transform_component.Up());
            set_uniform_3f(shader, "directional_light.ambient", light_component.ambient.value);
            set_uniform_3f(shader, "directional_light.diffuse", light_component.diffuse.value);
            set_uniform_3f(shader, "directional_light.specular", Vec3(light_component.specular));
        }
        set_uniform_3f(shader, "viewPos", context.camera_transform.position);
        if (skybox_identifier != nullptr)
        {
            Texture *texture = TextureLoaderSystem::get_texture(skybox_identifier);
            if (texture != nullptr)
                bind_texture(*texture, GL_TEXTURE0);
            glm::mat4 view = Mat4(glm::mat3(context.camera_transform.GetViewMatrix()));
            glm::mat4 MVP = projection * view;
            set_uniform_matrix_4fv(shader, "VP", glm::value_ptr(MVP));
            // shader->SetUniformMatrix4fv("view", glm::value_ptr());
        }
        else
        {
            for (auto [file, slot, name] : material.textures)
            {
                Texture *texture = TextureLoaderSystem::get_texture(file.path.string().c_str());
                if (texture != nullptr)
                    bind_texture(*texture, GL_TEXTURE0 + slot);
            }
            glm::mat4 view = context.camera_transform.GetViewMatrix();
            glm::mat4 MVP = projection * view * model;
            set_uniform_matrix_4fv(shader, "MVP", glm::value_ptr(MVP));
            // shader->SetUniformMatrix4fv("view", glm::value_ptr());
        }

        for (auto [uniform, value] : material.vec3s)
        {
            set_uniform_3f(shader, uniform.c_str(), value);
        }

        for (auto [uniform, value] : material.vec4s)
        {
            set_uniform_3f(shader, uniform.c_str(), value);
        }

        for (auto [uniform, value] : material.colors)
        {
            set_uniform_4f(shader, uniform.c_str(), value.value);
        }

        for (auto [uniform, value] : material.ints)
        {
            set_uniform_1i(shader, uniform.c_str(), value);
        }

        for (auto [uniform, value] : material.floats)
        {
            set_uniform_1f(shader, uniform.c_str(), value);
        }
    }

} // namespace Vultr::Renderer3D

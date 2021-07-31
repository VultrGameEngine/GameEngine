#include <rendering/renderers/forward_renderer_3d.h>
#include <core/system_providers/shader_loader_system_provider.h>
#include <core/system_providers/texture_loader_system_provider.h>
#include <core/system_providers/light_system_provider.h>
#include <glm/gtc/type_ptr.hpp>
#include <rendering/render_context.h>

namespace Vultr::Renderer3D
{
    void ForwardRenderer::submit(Engine *e, const MaterialComponent &material, const Mat4 &transform, const Mesh &mesh, const RenderContext &context, u32 skybox_identifier)
    {
        bind_material(e, material, transform, context, skybox_identifier);
        draw_mesh(mesh);
    }

    void ForwardRenderer::bind_material(Engine *e, const MaterialComponent &material, const Mat4 &transform, const RenderContext &context, u32 skybox_identifier)
    {
        Shader shader = ShaderLoaderSystem::get_shader(e, material.shader_source);
        if (!is_valid_shader(shader))
            return;
        bind_shader(shader);
        Mat4 model = transform;
        set_uniform_matrix_4fv(shader, "u_Model_matrix", glm::value_ptr(model));

        Mat3 normal_matrix = Mat3(glm::transpose(glm::inverse(model)));
        set_uniform_matrix_3fv(shader, "u_Normal_matrix", glm::value_ptr(normal_matrix));

        Entity directional_light = LightSystem::get_provider(e).directional_light;

        // TODO: Add this to a UBO
        if (directional_light != INVALID_ENTITY)
        {
            auto [transform_component, light_component] = entity_get_components<TransformComponent, LightComponent>(e, directional_light);
            set_uniform_3f(shader, "u_Directional_light.direction", -transform_component.Up());
            set_uniform_3f(shader, "u_Directional_light.ambient", light_component.ambient.value);
            set_uniform_3f(shader, "u_Directional_light.diffuse", light_component.diffuse.value);
            set_uniform_1f(shader, "u_Directional_light.specular", light_component.specular);
        }

        if (skybox_identifier != 0)
        {
            // Texture *texture = TextureLoaderSystem::get_texture(std::to_string(skybox_identifier));
            // if (texture != nullptr)
            //     bind_texture(*texture, GL_TEXTURE0);
            // glm::mat4 view = Mat4(glm::mat3(context.camera_transform.GetViewMatrix()));
            // glm::mat4 MVP = projection * view;
            // set_uniform_matrix_4fv(shader, "VP", glm::value_ptr(MVP));
            // shader->SetUniformMatrix4fv("view", glm::value_ptr());
        }
        else
        {
            for (s16 i = 0; i < material.texture_count; i++)
            {
                auto &texture_res = material.textures[i];
                Texture *texture = TextureLoaderSystem::get_texture(e, texture_res.file.path.string().c_str());
                if (texture != nullptr)
                    bind_texture(*texture, GL_TEXTURE0 + i);
            }
            glm::mat4 MVP = context.projection_matrix * context.view_matrix * model;
            set_uniform_matrix_4fv(shader, "u_MVP", glm::value_ptr(MVP));
        }

        material_bind_uniforms(material, shader);
    }

} // namespace Vultr::Renderer3D

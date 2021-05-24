#include <rendering/renderers/forward_renderer_3d.h>
#include <core/system_providers/shader_loader_system_provider.h>
#include <core/system_providers/texture_loader_system_provider.h>
#include <core/system_providers/light_system_provider.h>
#include <glm/gtc/type_ptr.hpp>
#include <rendering/render_context.h>

namespace Vultr::Renderer3D
{
    void ForwardRenderer::Submit(const MaterialComponent &material, const glm::mat4 &transform, const Mesh &mesh, const char *skybox_identifier)
    {
        BindMaterial(material, transform, skybox_identifier);
        mesh.Draw();
    }
    void ForwardRenderer::BindMaterial(const MaterialComponent &material, const glm::mat4 &transform, const char *skybox_identifier)
    {
        Shader *shader = ShaderLoaderSystem::get_shader(material.shader_source.GetPath().string().c_str());
        if (shader == nullptr)
            return;
        shader->Bind();
        shader->SetUniformMatrix4fv("model", glm::value_ptr(transform));

        const RenderContext &context = RenderContext::GetContext();

        shader->SetUniformMatrix4fv("projection", glm::value_ptr(context.camera_component.GetProjectionMatrix(context.dimensions.x, context.dimensions.y)));

        Entity directional_light = LightSystem::get_provider().directional_light;
        if (directional_light != INVALID_ENTITY)
        {
            auto &light_component = entity_get_component<LightComponent>(directional_light);
            auto &transform_component = entity_get_component<TransformComponent>(directional_light);
            shader->SetUniform3f("directional_light.direction", -transform_component.Up());
            shader->SetUniform3f("directional_light.ambient", light_component.ambient.value);
            shader->SetUniform3f("directional_light.diffuse", light_component.diffuse.value);
            shader->SetUniform3f("directional_light.specular", glm::vec3(light_component.specular));
        }
        shader->SetUniform3f("viewPos", context.camera_transform.position);
        if (skybox_identifier != nullptr)
        {
            Texture *texture = TextureLoaderSystem::get_texture(skybox_identifier);
            if (texture != nullptr)
                texture->Bind(GL_TEXTURE0);
            shader->SetUniformMatrix4fv("view", glm::value_ptr(glm::mat4(glm::mat3(context.camera_transform.GetViewMatrix()))));
        }
        else
        {
            for (auto [file, slot, name] : material.textures)
            {
                Texture *texture = TextureLoaderSystem::get_texture(file.GetPath().string().c_str());
                if (texture != nullptr)
                    texture->Bind(GL_TEXTURE0 + slot);
            }
            shader->SetUniformMatrix4fv("view", glm::value_ptr(context.camera_transform.GetViewMatrix()));
        }

        for (auto [uniform, value] : material.vec3s)
        {
            shader->SetUniform3f(uniform, value);
        }

        for (auto [uniform, value] : material.vec4s)
        {
            shader->SetUniform4f(uniform, value);
        }

        for (auto [uniform, value] : material.colors)
        {
            shader->SetUniform4f(uniform, value.value);
        }

        for (auto [uniform, value] : material.ints)
        {
            shader->SetUniform1i(uniform, value);
        }

        for (auto [uniform, value] : material.floats)
        {
            shader->SetUniform1f(uniform, value);
        }
    }

} // namespace Vultr::Renderer3D

#include <rendering/renderers/forward_renderer_3d.h>
#include <core/system_providers/shader_loader_system_provider.h>
#include <core/system_providers/texture_loader_system_provider.h>
#include <glm/gtc/type_ptr.hpp>
#include <rendering/render_context.h>

namespace Vultr::Renderer3D
{
    void ForwardRenderer::Submit(const MaterialComponent &material, const glm::mat4 &transform, const Mesh &mesh)
    {
        BindMaterial(material, transform);
        mesh.Draw();
    }
    void ForwardRenderer::BindMaterial(const MaterialComponent &material, const glm::mat4 &transform)
    {
        Shader *shader = ShaderLoaderSystem::get_shader(material.shader_path.c_str());
        if (shader == nullptr)
            return;
        shader->Bind();
        shader->SetUniformMatrix4fv("model", glm::value_ptr(transform));

        const RenderContext &context = RenderContext::GetContext();

        shader->SetUniformMatrix4fv("projection", glm::value_ptr(context.camera_component.GetProjectionMatrix(context.dimensions.x, context.dimensions.y)));
        shader->SetUniform3f("lightPos", context.light_position);
        shader->SetUniform3f("viewPos", context.camera_transform.position);
        for (auto [path, slot] : material.textures)
        {
            Texture *texture = TextureLoaderSystem::get_texture(path.c_str());
            if (texture != nullptr)
                texture->Bind(GL_TEXTURE0 + slot);
        }
        if (!material.identifier.empty())
        {
            shader->SetUniformMatrix4fv("view", glm::value_ptr(glm::mat4(glm::mat3(context.camera_transform.GetViewMatrix()))));
        }
        else
        {
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

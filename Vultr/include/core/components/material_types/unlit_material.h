#pragma once
#include <core/components/material_component.h>
#include <glm/gtc/type_ptr.hpp>
#include <rendering/render_context.h>

namespace Brick3D
{
struct UnlitMaterial : public MaterialComponent
{
    UnlitMaterial()
    {
        shader_path = "res/shaders/unlit.glsl";
    }

    static MaterialComponent &Create()
    {
        UnlitMaterial *mat = new UnlitMaterial();
        return *mat;
    }

    void BindShaders() const override
    {
        Shader *shader = GetShader();
        if (shader == nullptr)
            return;

        shader->Bind();
    }

    void SetModelUniforms(const glm::mat4 &transform) const override
    {
        Shader *shader = GetShader();
        if (shader == nullptr)
            return;
        const RenderContext &context = RenderContext::GetContext();

        shader->SetUniformMatrix4fv(
            "projection",
            glm::value_ptr(context.camera_component.GetProjectionMatrix(
                context.dimensions.x, context.dimensions.y)));
        shader->SetUniformMatrix4fv(
            "view", glm::value_ptr(context.camera_transform.GetViewMatrix()));

        shader->SetUniformMatrix4fv("model", glm::value_ptr(transform));
        shader->SetUniform3f("lightColor", glm::vec3(1, 1, 1));
    }

    void BindTextures() const override
    {
    }

    std::vector<std::string> GetTextures() const override
    {
        std::vector<std::string> textures;
        return textures;
    }

  private:
};
} // namespace Brick3D

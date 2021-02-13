#pragma once
#include <core/components/material_component.h>
#include <core/system_providers/texture_loader_system_provider.h>
#include <glm/gtc/type_ptr.hpp>
#include <rendering/models/texture.h>
#include <rendering/render_context.h>

namespace Brick3D
{
struct SkyboxMaterial : public MaterialComponent
{
    std::string identifier;

    SkyboxMaterial()
    {
        shader_path = "res/shaders/skybox.glsl";
    }

    static MaterialComponent &Create(const std::string &p_identifier)
    {
        SkyboxMaterial *mat = new SkyboxMaterial();
        mat->identifier = p_identifier;
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
            "view", glm::value_ptr(glm::mat4(
                        glm::mat3(context.camera_transform.GetViewMatrix()))));
        shader->SetUniform1i("skybox", 0);
    }

    void BindTextures() const override
    {
        Texture *texture = GetTexture();
        if (texture != nullptr)
            texture->Bind(GL_TEXTURE0);
    }

    std::vector<std::string> GetTextures() const override
    {
        std::vector<std::string> textures;
        return textures;
    }

    bool IsSkybox() const override
    {
        return true;
    }

  private:
    Texture *GetTexture() const
    {
        return TextureLoaderSystemProvider::Get().GetTexture(identifier);
    }
};
} // namespace Brick3D

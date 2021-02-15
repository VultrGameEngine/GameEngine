#pragma once
#include <core/components/material_component.h>
#include <core/system_providers/texture_loader_system_provider.h>
#include <glm/gtc/type_ptr.hpp>
#include <rendering/models/texture.h>
#include <rendering/render_context.h>

namespace Brick3D
{
struct PBRMaterial : public MaterialComponent
{
    std::string diffuse_path;
    std::string specular_path;

    PBRMaterial()
    {
        shader_path =
            "/home/brandon/Dev/GameEngine/SandboxTesting/res/shaders/material.glsl";
    }

    PBRMaterial(const std::string &p_diffuse_path,
                const std::string &p_specular_path)
        : diffuse_path(p_diffuse_path), specular_path(p_specular_path)
    {
        shader_path =
            "/home/brandon/Dev/GameEngine/SandboxTesting/res/shaders/material.glsl";
    }

    void BindShaders() const override
    {
        Shader *shader = GetShader();
        if (shader == nullptr)
            return;

        shader->Bind();

        const RenderContext &context = RenderContext::GetContext();

        shader->SetUniformMatrix4fv(
            "view", glm::value_ptr(context.camera_transform.GetViewMatrix()));

        shader->SetUniformMatrix4fv(
            "projection",
            glm::value_ptr(context.camera_component.GetProjectionMatrix(
                context.dimensions.x, context.dimensions.y)));
    }

    void SetModelUniforms(const glm::mat4 &transform) const override
    {
        Shader *shader = GetShader();
        if (shader == nullptr)
            return;
        shader->SetUniformMatrix4fv("model", glm::value_ptr(transform));
    }

    void BindTextures() const override
    {
        Texture *diffuse = GetDiffuse();
        if (diffuse != nullptr)
            diffuse->Bind(GL_TEXTURE0);

        Texture *specular = GetSpecular();
        if (specular != nullptr)
            specular->Bind(GL_TEXTURE1);

        Shader *shader = GetShader();
        if (shader == nullptr)
            return;
    }

    std::vector<std::string> GetTextures() const override
    {
        std::vector<std::string> textures;
        textures[0] = diffuse_path;
        textures[1] = specular_path;
        return textures;
    }

  private:
    Texture *GetDiffuse() const
    {
        return TextureLoaderSystemProvider::Get().GetTexture(diffuse_path);
    }

    Texture *GetSpecular() const
    {
        return TextureLoaderSystemProvider::Get().GetTexture(specular_path);
    }
};

} // namespace Brick3D

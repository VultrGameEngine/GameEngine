#pragma once
#include <core/components/material_component.h>
#include <core/system_providers/texture_loader_system_provider.h>
#include <glm/gtc/type_ptr.hpp>
#include <rendering/models/texture.h>
#include <rendering/render_context.h>
#include <cereal/types/polymorphic.hpp>

namespace Vultr
{
struct SkyboxMaterial : public MaterialComponent
{
    std::string identifier;

    SkyboxMaterial()
    {
        shader_path =
            "/home/brandon/Dev/GameEngine/SandboxTesting/res/shaders/skybox.glsl";
    }

    static std::shared_ptr<MaterialComponent> Create(
        const std::string &p_identifier = "default")
    {
        std::shared_ptr<SkyboxMaterial> mat = std::make_shared<SkyboxMaterial>();
        mat->identifier = p_identifier;
        return mat;
    }

    template <class Archive> void serialize(Archive &ar)
    {
        ar(cereal::base_class<MaterialComponent>(this), shader_path, identifier);
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
} // namespace Vultr
CEREAL_REGISTER_TYPE(SkyboxMaterial)

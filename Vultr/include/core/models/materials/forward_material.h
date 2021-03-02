#pragma once
#include <core/components/material_component.h>
#include <core/system_providers/texture_loader_system_provider.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <rendering/models/texture.h>
#include <rendering/render_context.h>
#include <memory>
#include <cereal/types/polymorphic.hpp>

namespace Vultr
{
struct ForwardMaterial : public MaterialComponent
{
    std::string texture_path;

    ForwardMaterial()
    {
        shader_path = "/home/brandon/Dev/GameEngine/SandboxTesting/res/shaders/"
                      "forward_material.glsl";
    }

    static std::shared_ptr<MaterialComponent> Create(
        const std::string &p_texture = "/home/brandon/Dev/GameEngine/SandboxTesting/"
                                       "res/textures/clone/albedo.jpeg")
    {
        std::shared_ptr<ForwardMaterial> mat = std::make_shared<ForwardMaterial>();
        mat->texture_path = p_texture;
        return mat;
    }

    template <class Archive> void serialize(Archive &ar)
    {
        ar(cereal::base_class<MaterialComponent>(this), shader_path, texture_path);
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
        shader->SetUniformMatrix4fv("model", glm::value_ptr(transform));

        const RenderContext &context = RenderContext::GetContext();
        shader->SetUniformMatrix4fv(
            "projection",
            glm::value_ptr(context.camera_component.GetProjectionMatrix(
                context.dimensions.x, context.dimensions.y)));
        shader->SetUniform3f("lightPos", context.light_position);
        shader->SetUniform3f("viewPos", context.camera_transform.position);
        shader->SetUniform3f("objectColor", glm::vec3(1, 1, 1));
        shader->SetUniform3f("lightColor", glm::vec3(1, 1, 1));
        shader->SetUniformMatrix4fv(
            "view", glm::value_ptr(context.camera_transform.GetViewMatrix()));

        // Slot 0 for texture
        shader->SetUniform1i("textureSampler", 0);
    }

    void BindTextures() const override
    {
        Texture *diffuse = GetAlbedo();
        if (diffuse != nullptr)
            diffuse->Bind(GL_TEXTURE0);
    }

    std::vector<std::string> GetTextures() const override
    {
        std::vector<std::string> textures;
        textures.push_back(texture_path);
        return textures;
    }

  private:
    Texture *GetAlbedo() const
    {
        return TextureLoaderSystemProvider::Get().GetTexture(texture_path);
    }
};
} // namespace Vultr
CEREAL_REGISTER_TYPE(ForwardMaterial)

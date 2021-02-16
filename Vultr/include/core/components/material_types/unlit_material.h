#pragma once
#include <core/components/material_component.h>
#include <glm/gtc/type_ptr.hpp>
#include <rendering/render_context.h>
#include <cereal/types/polymorphic.hpp>

namespace Vultr
{
struct UnlitMaterial : public MaterialComponent
{
    UnlitMaterial()
    {
        shader_path =
            "/home/brandon/Dev/GameEngine/SandboxTesting/res/shaders/unlit.glsl";
    }

    static std::shared_ptr<MaterialComponent> Create()
    {
        std::shared_ptr<UnlitMaterial> mat = std::make_shared<UnlitMaterial>();
        return mat;
    }

    template <class Archive> void serialize(Archive &ar)
    {
        ar(cereal::base_class<MaterialComponent>(this), shader_path);
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
} // namespace Vultr

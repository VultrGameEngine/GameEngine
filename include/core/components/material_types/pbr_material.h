#pragma once
#include <core/components/material_component.h>
#include <rendering/models/texture.h>

namespace Brick3D
{
struct PBRMaterial : public MaterialComponent
{
    std::string diffuse_path;
    std::string specular_path;

    PBRMaterial(std::string p_shader_path, std::string p_diffuse_path,
                std::string p_specular_path)
        : diffuse_path(p_diffuse_path), specular_path(p_specular_path)
    {
        shader_path = p_shader_path;
    }

    bool Bind() override
    {
        Shader *shader = GetShader();
        if (shader == nullptr)
        {
            return false;
        }

        shader->Bind();

        return true;
    }

  private:
    Texture *GetDiffuse()
    {
    }

    Texture *GetSpecular()
    {
    }
};

} // namespace Brick3D

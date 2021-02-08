#pragma once
#include <core/components/material_component.h>
#include <core/system_providers/texture_loader_system_provider.h>
#include <rendering/models/texture.h>

namespace Brick3D
{
struct PBRMaterial : public MaterialComponent
{
    std::string diffuse_path;
    std::string specular_path;

    PBRMaterial()
    {
    }

    PBRMaterial(const std::string &p_shader_path, const std::string &p_diffuse_path,
                const std::string &p_specular_path)
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

    std::vector<std::string> GetTextures() override
    {
        std::vector<std::string> textures;
        textures[0] = diffuse_path;
        textures[1] = specular_path;
        return textures;
    }

  private:
    Texture *GetDiffuse()
    {
        return TextureLoaderSystemProvider::Get().GetTexture(diffuse_path);
    }

    Texture *GetSpecular()
    {
        return TextureLoaderSystemProvider::Get().GetTexture(specular_path);
    }
};

} // namespace Brick3D

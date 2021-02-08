#pragma once
#include <core/system_providers/shader_loader_system_provider.h>
#include <rendering/models/shader.h>
#include <string>
#include <vector>

namespace Brick3D
{
struct MaterialComponent
{
    std::string shader_path;
    virtual RenderType GetRenderType()
    {
        return RenderType::Deferred;
    }

    virtual bool Bind()
    {
        assert("DON'T USE THIS COMPONENT DIRECTLY");
        return false;
    };
    Shader *GetShader()
    {
        return ShaderLoaderSystemProvider::GetShader(shader_path);
    }

    virtual std::vector<std::string> GetTextures()
    {
        assert("DON'T USE THIS COMPONENT DIRECTLY");
        std::vector<std::string> placeholder;
        return placeholder;
    }
};
} // namespace Brick3D

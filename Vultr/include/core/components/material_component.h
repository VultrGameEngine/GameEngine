#pragma once
#include <core/components/transform_component.h>
#include <core/system_providers/shader_loader_system_provider.h>
#include <iostream>
#include <rendering/models/shader.h>
#include <string>
#include <vector>

namespace Vultr
{
struct MaterialComponent
{
    std::string shader_path;
    virtual RenderType GetRenderType()
    {
        return RenderType::Deferred;
    }

    virtual void BindShaders() const
    {
        assert("DON'T USE THIS COMPONENT DIRECTLY");
        std::cout << "TEST" << std::endl;
    };

    virtual void SetModelUniforms(const glm::mat4 &transform) const
    {
        assert("DON'T USE THIS COMPONENT DIRECTLY");
    }

    virtual void BindTextures() const
    {
        assert("DON'T USE THIS COMPONENT DIRECTLY");
    }

    Shader *GetShader() const
    {
        return ShaderLoaderSystemProvider::GetShader(shader_path);
    }

    virtual std::vector<std::string> GetTextures() const
    {
        assert("DON'T USE THIS COMPONENT DIRECTLY");
        std::vector<std::string> placeholder;
        return placeholder;
    }

    virtual bool IsSkybox() const
    {
        return false;
    }
};
} // namespace Vultr

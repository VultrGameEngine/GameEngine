#pragma once
#include <core/components/transform_component.h>
#include <core/system_providers/shader_loader_system_provider.h>
#include <iostream>
#include <rendering/models/shader.h>
#include <string>
#include <vector>
#include <cereal/types/vector.hpp>

namespace Vultr
{
struct MaterialComponent
{
    std::string shader_path;
    std::vector<std::string> textures;
    std::vector<glm::vec3> vec3s;

    static MaterialComponent Create()
    {
        return MaterialComponent();
    }

    bool is_skybox = false;

    template <class Archive> void serialize(Archive &ar)
    {
        ar(shader_path, textures);
    }

    void BindShaders() const {};

    void SetModelUniforms(const glm::mat4 &transform) const
    {
    }

    void BindTextures() const
    {
    }

    Shader *GetShader() const
    {
        return ShaderLoaderSystemProvider::GetShader(shader_path);
    }
};
} // namespace Vultr

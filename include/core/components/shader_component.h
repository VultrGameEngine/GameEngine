#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <core/system_providers/shader_loader_system_provider.h>
#include <string>
namespace Brick3D
{
struct ShaderComponent
{
    std::string path;

    Shader *GetShader()
    {
        return ShaderLoaderSystemProvider::GetShader(this->path);
    }
};
} // namespace Brick3D

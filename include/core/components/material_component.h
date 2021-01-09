#pragma once
#include <rendering/models/shader.h>
#include <string>

namespace Brick3D
{
struct MaterialComponent
{
    std::string shader_path;

    virtual bool Bind() = 0;
    Shader *GetShader()
    {
    }
};
} // namespace Brick3D

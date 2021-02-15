#pragma once
#include <rendering/models/shader.h>
#include <string>

namespace Vultr
{
struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};
class ShaderImporter
{
  public:
    static Shader *ImportShader(const std::string &path);
    static ShaderProgramSource ParseShader(const std::string &filepath);
    static unsigned int CreateShader(const std::string &path);

    static unsigned int CompileShader(unsigned int type, const std::string &source);
};

} // namespace Vultr

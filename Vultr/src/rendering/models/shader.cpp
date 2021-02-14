#include <glm/gtc/type_ptr.hpp>
#include <rendering/models/shader.h>

namespace Brick3D
{
Shader::Shader()
{
}

Shader::Shader(unsigned int p_id, RenderType p_type) : id(p_id), type(p_type)
{
}

Shader::~Shader()
{
    glDeleteProgram(this->id);
}

void Shader::Bind()
{
    glUseProgram(this->id);
}

void Shader::Unbind()
{
    glUseProgram(0);
}

void Shader::SetUniformMatrix4fv(const std::string &uniform, const float *value)
{
    glUniformMatrix4fv(GetUniformLocation(uniform), 1, GL_FALSE, value);
}

void Shader::SetUniform3f(const std::string &uniform, const glm::vec3 &value)
{
    glUniform3f(GetUniformLocation(uniform), value.x, value.y, value.z);
}

void Shader::SetUniform2f(const std::string &uniform, const glm::vec2 &value)
{
    glUniform2f(GetUniformLocation(uniform), value.x, value.y);
}

void Shader::SetUniform1i(const std::string &uniform, int value)
{
    glUniform1i(GetUniformLocation(uniform), value);
}

void Shader::SetUniform1f(const std::string &uniform, float value)
{
    glUniform1f(GetUniformLocation(uniform), value);
}

unsigned int Shader::GetUniformLocation(const std::string &uniform)
{
    return glGetUniformLocation(this->id, uniform.c_str());
}

} // namespace Brick3D

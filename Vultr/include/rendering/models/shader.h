#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <rendering/render_type.h>
#include <string>

namespace Brick3D
{
class Shader
{
  public:
    Shader();
    Shader(unsigned int p_id, RenderType p_type);
    ~Shader();
    void SetUniformMatrix4fv(const std::string &uniform, const float *value);

    void SetUniform3f(const std::string &uniform, const glm::vec3 &value);

    void SetUniform2f(const std::string &uniform, const glm::vec2 &value);

    void SetUniform1i(const std::string &uniform, int value);

    void SetUniform1f(const std::string &uniform, float value);

    unsigned int GetUniformLocation(const std::string &uniform);

    void Bind();
    void Unbind();

  private:
    unsigned int id = 0;
    RenderType type = Deferred;
};
} // namespace Brick3D

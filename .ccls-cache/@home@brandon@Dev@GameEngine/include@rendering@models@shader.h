#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <rendering/render_type.h>

namespace Brick3D
{
class Shader
{
  public:
    Shader();
    Shader(unsigned int p_id, RenderType p_type);
    ~Shader();

    void Bind();
    void Unbind();

  private:
    unsigned int id = 0;
    RenderType type = Deferred;
};
} // namespace Brick3D

#pragma once
#include "GL/gl.h"
#include "GLFW/glfw3.h"

namespace Renderer {
class Shader {
public:
  Shader(GLuint id);
  ~Shader();

  // Bind the shader
  void Bind(unsigned int slot);

  // Unbind the shader
  void Unbind(unsigned int slot);

private:
  GLuint id;
};
}; // namespace Renderer

#pragma once
#include "GL/gl.h"
#include "GLFW/glfw3.h"

namespace Renderer {
class Texture {
public:
  Texture(GLuint id);
  ~Texture();

  // Bind the texture
  void Bind(unsigned int slot);

  // UnBind the texture
  void Unbind(unsigned int slot);

private:
  GLuint id;
};
}; // namespace Renderer

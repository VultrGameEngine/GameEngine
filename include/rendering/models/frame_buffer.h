#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Renderer {
class FrameBuffer {
public:
  void Bind() { glBindFramebuffer(GL_FRAMEBUFFER, this->id); }
  void BindRead() { glBindFramebuffer(GL_READ_FRAMEBUFFER, this->id); }
  void BindWrite() { glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->id); }
  void Unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
  void UnbindRead() { glBindFramebuffer(GL_READ_FRAMEBUFFER, 0); }
  void UnbindWrite() { glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); }

  unsigned int GetID() { return this->id; }
  FrameBuffer() {}
  ~FrameBuffer() {}

protected:
  unsigned int id = 0;
};
} // namespace Renderer

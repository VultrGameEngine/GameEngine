#pragma once
#include "frame_buffer.h"

namespace Renderer {
class GBuffer : public FrameBuffer {
public:
  GBuffer() {}
  ~GBuffer() {}

  void Init(int width, int height) {
    glGenFramebuffers(1, &this->id);
    this->Bind();

    // Position color buffer
    glGenTextures(1, &this->position);
    glBindTexture(GL_TEXTURE_2D, this->position);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA,
                 GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           position, 0);

    // - normal color buffer
    glGenTextures(1, &this->normal);
    glBindTexture(GL_TEXTURE_2D, this->normal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA,
                 GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D,
                           this->normal, 0);

    // - color + specular color buffer
    glGenTextures(1, &this->color_specular);
    glBindTexture(GL_TEXTURE_2D, this->color_specular);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D,
                           this->color_specular, 0);

    // Tell opengl with color attachmnts we're using for rendering
    unsigned int attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
                                   GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, attachments);

    // Generate the render buffer for depth
    glGenRenderbuffers(1, &this->rbo_depth);
    glBindRenderbuffer(GL_RENDERBUFFER, this->rbo_depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, this->rbo_depth);
  }

  void Resize(int width, int height) {
    if (this->position != 0) {
      glDeleteFramebuffers(1, &this->id);
      glDeleteTextures(1, &this->position);
      glDeleteTextures(1, &this->normal);
      glDeleteTextures(1, &this->color_specular);
      glDeleteRenderbuffers(1, &this->rbo_depth);
    }
    Init(width, height);
  }

  void BindTextures() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, position);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, color_specular);
  }

private:
  unsigned int position = 0;
  unsigned int normal = 0;
  unsigned int color_specular = 0;
  unsigned int rbo_depth = 0;

  int width = 1920;
  int height = 1080;
};
} // namespace Renderer

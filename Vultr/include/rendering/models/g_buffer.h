#pragma once
#include "frame_buffer.h"

namespace Vultr
{
class GBuffer : public FrameBuffer
{
  public:
    GBuffer()
    {
        pnc_attachments = new unsigned int[3]{
            GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
        light_attachments = new unsigned int[1]{GL_COLOR_ATTACHMENT3};
    }
    ~GBuffer()
    {
        glDeleteFramebuffers(1, &this->id);
        glDeleteTextures(1, &this->position);
        glDeleteTextures(1, &this->normal);
        glDeleteTextures(1, &this->color_specular);
        glDeleteRenderbuffers(1, &this->rbo_depth);

        delete[] pnc_attachments;
        delete[] light_attachments;
    }

    void Init(int width, int height)
    {
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

        // - light buffer
        glGenTextures(1, &this->light);
        glBindTexture(GL_TEXTURE_2D, this->light);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA,
                     GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D,
                               this->light, 0);

        SetWritePNC();

        // Generate the render buffer for depth
        glGenRenderbuffers(1, &this->rbo_depth);
        glBindRenderbuffer(GL_RENDERBUFFER, this->rbo_depth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                  GL_RENDERBUFFER, this->rbo_depth);
    }

    void Resize(int width, int height)
    {
        if (this->position != 0)
        {
            glDeleteFramebuffers(1, &this->id);
            glDeleteTextures(1, &this->position);
            glDeleteTextures(1, &this->normal);
            glDeleteTextures(1, &this->color_specular);
            glDeleteRenderbuffers(1, &this->rbo_depth);
        }
        Init(width, height);
    }

    void BindPositionTexture(unsigned int slot = 0)
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, position);
    }

    void BindNormalTexture(unsigned int slot = 1)
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, normal);
    }

    void BindColorSpecularTexture(unsigned int slot = 2)
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, color_specular);
    }

    void BindLightSpecularTexture(unsigned int slot = 3)
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, light);
    }

    // Set the draw buffers to be the postion, color, and normals
    void SetWritePNC()
    {
        // Tell opengl with color attachmnts we're using for rendering
        glDrawBuffers(3, pnc_attachments);
    }

    // Set the draw buffers to be the light
    void SetWriteLight()
    {
        glDrawBuffer(GL_COLOR_ATTACHMENT3);
    }

    void SetWriteNone()
    {
        glDrawBuffer(GL_NONE);
    }

  private:
    unsigned int position = 0;
    unsigned int normal = 0;
    unsigned int color_specular = 0;
    unsigned int light = 0;
    unsigned int rbo_depth = 0;

    unsigned int *pnc_attachments;
    unsigned int *light_attachments;

    int width = 1920;
    int height = 1080;
};
} // namespace Vultr

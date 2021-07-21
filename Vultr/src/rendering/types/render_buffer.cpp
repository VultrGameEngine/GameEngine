// #include <glad/glad.h>
// #include <iostream>
// #include <rendering/types/render_buffer.h>

// namespace Vultr
// {
// RenderBuffer::RenderBuffer(unsigned int width, unsigned int height)
// {
//     glCreateRenderbuffers(1, &this->m_id);
//     Generate(width, height);
// }

// RenderBuffer::~RenderBuffer()
// {
//     glDeleteRenderbuffers(1, &this->m_id);
// }

// void RenderBuffer::Generate(unsigned int width, unsigned int height)
// {
//     this->Bind();
//     this->m_width = width;
//     this->m_height = height;

//     glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width,
//                           height); // use a single renderbuffer object for both a
//                                    // depth AND stencil buffer.
//     glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
//                               GL_RENDERBUFFER,
//                               this->m_id); // now actually attach it
//     if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//     {
//         std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!"
//                   << std::endl;
//     }
// }

// void RenderBuffer::Bind()
// {
//     glBindRenderbuffer(GL_RENDERBUFFER, this->m_id);
// }

// void RenderBuffer::Unbind()
// {
//     glBindRenderbuffer(GL_RENDERBUFFER, 0);
// }

// } // namespace Vultr

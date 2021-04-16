#pragma once

namespace Vultr
{
class RenderBuffer
{
  public:
    RenderBuffer(unsigned int width, unsigned int height);
    ~RenderBuffer();

    void Generate(unsigned int width, unsigned int height);

    void Bind();
    void Unbind();

  private:
    unsigned int m_id;
    unsigned int m_width;
    unsigned int m_height;
};
} // namespace Vultr

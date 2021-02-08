#pragma once

namespace Brick3D
{
class RenderBuffer
{
  public:
    RenderBuffer();
    ~RenderBuffer();

    void Generate(unsigned int width, unsigned int height);

    void Bind();
    void Unbind();

  private:
    unsigned int m_id;
    unsigned int m_width;
    unsigned int m_height;
};
} // namespace Brick3D

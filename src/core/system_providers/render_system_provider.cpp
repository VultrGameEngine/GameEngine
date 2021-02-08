#include <core/system_providers/render_system_provider.h>

namespace Brick3D
{

glm::vec2 RenderSystemProvider::GetDimensions(unsigned int type)
{
    if (type == SCENE)
    {
        return Get().scene.dimensions;
    }
    else
    {
        return Get().game.dimensions;
    }
}
void RenderSystemProvider::GenerateRenderTexture(FrameBuffer *fbo,
                                                 Texture *render_texture,
                                                 RenderBuffer *rbo, int width,
                                                 int height)
{
    if (fbo != nullptr)
        delete fbo;
    if (render_texture != nullptr)
        delete render_texture;
    if (rbo != nullptr)
        delete rbo;

    fbo = new FrameBuffer();
    fbo->Bind();
    render_texture = new Texture(GL_TEXTURE_2D);
    render_texture->Bind();
    render_texture->Generate(width, height);
    render_texture->FrameBufferTexture2D();

    rbo = new RenderBuffer();
    rbo->Bind();
    rbo->Generate(width, height);

    fbo->Unbind();
}
} // namespace Brick3D

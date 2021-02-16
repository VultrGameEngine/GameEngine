#include <core/system_providers/render_system_provider.h>
#include <helpers/shader_importer.h>

namespace Vultr
{

void RenderSystemProvider::InitGBuffer(int width, int height)
{
    // if (Get().g_buffer == nullptr)
    // {
    //     Get().g_buffer = new GBuffer();
    //     Get().g_buffer->Init(width, height);
    // }

    // InitQuad();
    //     InitSphere();
    // if (Get().lighting_pass_shader == nullptr)
    //     Get().lighting_pass_shader =
    //         ShaderImporter::ImportShader("res/shaders/lighting_pass.glsl");

    // if (Get().stencil_pass_shader == nullptr)
    //     Get().stencil_pass_shader =
    //         ShaderImporter::ImportShader("res/shaders/stencil.glsl");

    // if (Get().composite_pass_shader == nullptr)
    //     Get().composite_pass_shader =
    //         ShaderImporter::ImportShader("res/shaders/composite_pass.glsl");
}

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
void RenderSystemProvider::GenerateRenderTexture(ViewportData &data, int width,
                                                 int height)
{
    if (data.fbo != nullptr)
        delete data.fbo;
    if (data.render_texture != nullptr)
        delete data.render_texture;
    if (data.rbo != nullptr)
        delete data.rbo;

    data.fbo = new FrameBuffer();
    data.fbo->Bind();
    data.render_texture = new Texture(GL_TEXTURE_2D);
    data.render_texture->Bind(GL_TEXTURE0);
    data.render_texture->Generate(width, height);
    data.render_texture->FrameBufferTexture2D();

    data.rbo = new RenderBuffer(width, height);

    data.fbo->Unbind();
}
void RenderSystemProvider::Resize(int width, int height, unsigned int type)
{
    Get().InitGBuffer(width, height);
    if (type == GAME)
    {
        if (Get().game.dimensions == glm::vec2(width, height))
        {
            return;
        }
        Get().game.dimensions = glm::vec2(width, height);
        Get().GenerateRenderTexture(Get().game, width, height);
    }
    else if (type == SCENE)
    {
        if (Get().scene.dimensions == glm::vec2(width, height))
        {
            return;
        }
        Get().scene.dimensions = glm::vec2(width, height);
        Get().GenerateRenderTexture(Get().scene, width, height);
    }
}
} // namespace Vultr

#include <core/system_providers/render_system_provider.h>
#include <engine.hpp>

namespace Vultr::RenderSystem
{
    Component &get_provider()
    {
        return *get_global_system_provider<Component>();
    }

    glm::vec2 get_dimensions(u8 type)
    {
        assert((type == GAME || type == SCENE) && "Please call this method with a valid type, either GAME or SCENE");

        if (type == GAME)
        {
            return get_provider().game.dimensions;
        }
        else
        {
            return get_provider().scene.dimensions;
        }
    }

    // void RenderSystemProvider::InitGBuffer(int width, int height)
    // {
    //     // if (Get().g_buffer == nullptr)
    //     // {
    //     //     Get().g_buffer = new GBuffer();
    //     //     Get().g_buffer->Init(width, height);
    //     // }

    //     // InitQuad();
    //     //     InitSphere();
    //     // if (Get().lighting_pass_shader == nullptr)
    //     //     Get().lighting_pass_shader =
    //     //         ShaderImporter::ImportShader("res/shaders/lighting_pass.glsl");

    //     // if (Get().stencil_pass_shader == nullptr)
    //     //     Get().stencil_pass_shader =
    //     //         ShaderImporter::ImportShader("res/shaders/stencil.glsl");

    //     // if (Get().composite_pass_shader == nullptr)
    //     //     Get().composite_pass_shader =
    //     //         ShaderImporter::ImportShader("res/shaders/composite_pass.glsl");
    // }

    // glm::vec2 RenderSystemProvider::GetDimensions(unsigned int type)
    // {
    //     if (type == SCENE)
    //     {
    //         return Get()->scene.dimensions;
    //     }
    //     else
    //     {
    //         return Get()->game.dimensions;
    //     }
    // }
    // void RenderSystemProvider::GenerateRenderTexture(ViewportData &data, int width, int height)
    // {
    //     if (data.fbo != nullptr)
    //         delete data.fbo;
    //     if (data.render_texture != nullptr)
    //         delete data.render_texture;
    //     if (data.rbo != nullptr)
    //         delete data.rbo;

    //     data.fbo = new FrameBuffer();
    //     data.fbo->Bind();
    //     data.render_texture = new Texture(GL_TEXTURE_2D);
    //     data.render_texture->Bind(GL_TEXTURE0);
    //     data.render_texture->Generate(width, height);
    //     data.render_texture->FrameBufferTexture2D();

    //     data.rbo = new RenderBuffer(width, height);

    //     data.fbo->Unbind();
    //     data.dimensions.x = width;
    //     data.dimensions.y = height;
    // }
    // void RenderSystemProvider::Resize(int width, int height, unsigned int type)
    // {
    //     Get()->InitGBuffer(width, height);
    //     if (type == GAME)
    //     {
    //         if (Get()->game.dimensions == glm::vec2(width, height))
    //         {
    //             return;
    //         }
    //         Get()->game.dimensions = glm::vec2(width, height);
    //         Get()->GenerateRenderTexture(Get()->game, width, height);
    //     }
    //     else if (type == SCENE)
    //     {
    //         if (Get()->scene.dimensions == glm::vec2(width, height))
    //         {
    //             return;
    //         }
    //         Get()->scene.dimensions = glm::vec2(width, height);
    //         Get()->GenerateRenderTexture(Get()->scene, width, height);
    //         Get()->GenInputFB(width, height);
    //     }
    // }

    // void RenderSystemProvider::UpdateViewportPos(int x, int y, unsigned int type)
    // {
    //     if (type == GAME)
    //     {
    //         Get()->game.position = glm::vec2(x, y);
    //     }
    //     else if (type == SCENE)
    //     {
    //         Get()->scene.position = glm::vec2(x, y);
    //     }
    // }

    // void RenderSystemProvider::GenInputFB(uint width, uint height)
    // {
    //     if (input_data.fb != nullptr)
    //         delete input_data.fb;
    //     if (input_data.render_texture != nullptr)
    //         delete input_data.render_texture;
    //     if (input_data.rbo != nullptr)
    //         delete input_data.rbo;
    //     input_data.fb = new FrameBuffer();
    //     input_data.fb->Bind();
    //     input_data.render_texture = new Texture(GL_TEXTURE_2D);
    //     input_data.render_texture->Bind(GL_TEXTURE0);
    //     input_data.render_texture->Generate(width, height);
    //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //     input_data.render_texture->FrameBufferTexture2D();

    //     input_data.rbo = new RenderBuffer(width, height);

    //     input_data.fb->Unbind();
    // }

} // namespace Vultr::RenderSystem

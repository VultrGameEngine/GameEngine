#pragma once
#include <ecs/system/system_provider.hpp>
#include <rendering/models/frame_buffer.h>
#include <rendering/models/texture.h>
#include <rendering/models/render_buffer.h>
#include <rendering/models/mesh.h>
#include <rendering/models/shader.h>
#include <type_info/type_info.h>

const u8 GAME = 0;
const u8 SCENE = 1;

namespace Vultr
{

    namespace RenderSystem
    {
        // Struct containing all of the texture data that will be rendered to
        // Mostly for the editor atm...
        struct ViewportData
        {
            glm::vec2 dimensions = glm::vec2(1920, 1080);
            glm::vec2 position = glm::vec2(0);
            FrameBuffer *fbo = nullptr;
            Texture *render_texture = nullptr;
            RenderBuffer *rbo = nullptr;
        };

        struct Component : public SystemProvider
        {
            ViewportData scene;
            ViewportData game;
            ViewportData input_data;

            Shader *post_processing_shader;
            Mesh *render_quad;
        };

        Component &get_provider();

        glm::vec2 get_dimensions(u8 type);
    } // namespace RenderSystem
    template <>
    inline const char *get_struct_name<RenderSystem::Component>()
    {
        return "RenderSystem";
    }
    // class RenderSystemProvider : public SystemProvider
    // {
    //   public:
    //     RenderSystemProvider()
    //     {
    //     }
    //     // Singleton pattern
    //     static std::shared_ptr<RenderSystemProvider> Get()
    //     {
    //         return Engine::GetSystemProvider<RenderSystemProvider>();
    //     }

    //     static void InitGBuffer(int width, int height);
    //     static glm::vec2 GetDimensions(unsigned int type);
    //     static void GenerateRenderTexture(ViewportData &data, int width, int height);
    //     static void Resize(int width, int height, unsigned int type);
    //     static void UpdateViewportPos(int x, int y, unsigned int type);

    //     ViewportData scene;
    //     ViewportData game;

    //     Shader *post_processing_shader;
    //     Mesh *render_quad;

    //     int GetEntityAtPixel(int x, int y)
    //     {
    //         input_data.fb->Bind();
    //         glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    //         unsigned char data[4];
    //         glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
    //         int pickedID = data[0] + data[1] * 256 + data[2] * 256 * 256;
    //         input_data.fb->Unbind();
    //         return pickedID;
    //     }

    //   protected:
    //     void OnCreateEntity(Entity entity) override;
    //     void OnDestroyEntity(Entity entity) override;
    //     Mesh *m_camera_mesh;
    //     MaterialComponent camera_mat;
    //     Shader *input_shader;
    //     struct InputData
    //     {
    //         FrameBuffer *fb = nullptr;
    //         Texture *render_texture = nullptr;
    //         RenderBuffer *rbo = nullptr;
    //     } input_data;

    //     friend RenderSystem;

    //   private:
    //     void GenInputFB(uint width, uint height);
    // };

} // namespace Vultr

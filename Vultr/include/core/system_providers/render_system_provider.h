#pragma once
#include <ecs/system/system_provider.hpp>
#include <rendering/models/frame_buffer.h>
#include <rendering/models/texture.h>
#include <rendering/models/render_buffer.h>
#include <rendering/models/mesh.h>
#include <rendering/models/shader.h>
#include <type_info/type_info.h>
#include <fundamental/types.h>

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
            Vec2 dimensions = Vec2(1920, 1080);
            Vec2 position = Vec2(0);
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
            Shader *input_shader;
            Mesh *render_quad;
            Mesh *skybox;
        };

        Component &get_provider();

        Vec2 get_dimensions(u8 type);

        Entity get_entity_at_pixel(s32 x, s32 y);

    } // namespace RenderSystem
    template <>
    inline const char *get_struct_name<RenderSystem::Component>()
    {
        return "RenderSystem";
    }
} // namespace Vultr

#pragma once
#include <engine.hpp>
#include <ecs/system/system_provider.hpp>
#include <rendering/types/framebuffer.h>
#include <rendering/types/texture.h>
#include <rendering/types/mesh.h>
#include <rendering/types/shader.h>
#include <type_info/type_info.h>
#include <fundamental/types.h>

const u8 GAME = 0;
const u8 SCENE = 1;

namespace Vultr
{
    namespace RenderSystem
    {
        struct ViewportData
        {
            Vec2 position = Vec2(0);
            Vec2 dimensions = Vec2(1920, 1080);
            Framebuffer render_fbo = invalid_framebuffer();
            Framebuffer post_processed_fbo = invalid_framebuffer();
            Framebuffer ping_pong_blur_framebuffers[2] = {invalid_framebuffer(), invalid_framebuffer()};
        };

        struct Component : public SystemProvider
        {
            ViewportData scene;
            ViewportData game;
            ViewportData input_data;

            Shader post_processing_shader = invalid_shader();
            Shader input_shader = invalid_shader();
            Shader gaussian_blur_shader = invalid_shader();

            Mesh render_quad;
            Mesh skybox;
        };

        Component &get_provider(Engine *e);

        Vec2 get_dimensions(Engine *e, u8 type);

        Entity get_entity_at_pixel(Engine *e, s32 x, s32 y);

    } // namespace RenderSystem

    template <>
    inline const char *get_struct_name<RenderSystem::Component>()
    {
        return "RenderSystem";
    }
} // namespace Vultr

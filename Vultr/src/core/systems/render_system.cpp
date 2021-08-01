#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <core/components/material_component.h>
#include <core/components/sky_box_component.h>
#include <core/components/static_mesh_component.h>
#include <core/system_providers/shader_loader_system_provider.h>
#include <core/system_providers/camera_system_provider.h>
#include <core/system_providers/light_system_provider.h>
#include <core/system_providers/render_system_provider.h>
#include <core/systems/shader_loader_system.h>
#include <core/systems/texture_loader_system.h>
#include <core/system_providers/mesh_loader_system_provider.h>
#include <core/systems/gui_system.h>
#include <helpers/shader_importer.h>
#include <helpers/mesh_importer.h>
#include <core/systems/render_system.h>
#include <core/system_providers/input_system_provider.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <rendering/renderers/forward_renderer_3d.h>
#include <core/models/update_tick.h>
#include <rendering/render_context.h>

namespace Vultr::RenderSystem
{
    // Internal private helper methods
    static void render_skybox(Engine *e, const RenderContext &context, u8 type);
    static void render_elements(Engine *e, const RenderContext &context, u8 type);
    static void render_element_input(Engine *e, const RenderContext &context);
    static void init_render_texture(ViewportData &d, u32 width, u32 height)
    {
        {
            d.render_fbo = new_framebuffer(width, height);

            // Attach our color texture
            auto color_texture = generate_texture(GL_TEXTURE_2D);
            attach_color_texture_framebuffer(d.render_fbo, color_texture, 0, GL_RGBA16F, GL_RGBA, GL_FLOAT);

            // Attach a brightness texture for bloom
            auto bright_texture = generate_texture(GL_TEXTURE_2D);
            attach_color_texture_framebuffer(d.render_fbo, color_texture, 1, GL_RGBA16F, GL_RGBA, GL_FLOAT);

            // Attach our depth/stencil rbo
            auto rbo = new_renderbuffer();
            attach_stencil_depth_renderbuffer_framebuffer(d.render_fbo, rbo);

            confirm_complete_framebuffer(d.render_fbo);
        }
        // Initialize bloom frame buffers
        {
            for (s8 i = 0; i < 2; i++)
            {
                auto &fbo = d.ping_pong_blur_framebuffers[i];
                fbo = new_framebuffer(width, height);

                auto texture = generate_texture(GL_TEXTURE_2D);
                attach_color_texture_framebuffer(fbo, texture, 0, GL_RGBA16F, GL_RGBA, GL_FLOAT);

                // Attach our depth/stencil rbo
                auto rbo = new_renderbuffer();
                attach_stencil_depth_renderbuffer_framebuffer(fbo, rbo);

                confirm_complete_framebuffer(fbo);
            }
        }
        {
            d.post_processed_fbo = new_framebuffer(width, height);

            // Attach our color texture
            auto texture = generate_texture(GL_TEXTURE_2D);
            attach_color_texture_framebuffer(d.post_processed_fbo, texture, 0, GL_RGBA16F, GL_RGBA, GL_FLOAT);

            // Attach our depth/stencil rbo
            auto rbo = new_renderbuffer();
            attach_stencil_depth_renderbuffer_framebuffer(d.post_processed_fbo, rbo);

            confirm_complete_framebuffer(d.post_processed_fbo);
        }
    }

    static void init_input_texture(ViewportData &d, u32 width, u32 height)
    {
        d.render_fbo = new_framebuffer(width, height);

        // Attach our color texture
        auto texture = generate_texture(GL_TEXTURE_2D);
        attach_color_texture_framebuffer(d.render_fbo, texture, 0, GL_RGBA16F, GL_RGBA, GL_FLOAT);

        // Attach our depth/stencil rbo
        auto rbo = new_renderbuffer();
        attach_stencil_depth_renderbuffer_framebuffer(d.render_fbo, rbo);

        confirm_complete_framebuffer(d.render_fbo);
    }

    void register_system(Engine *e)
    {
        Signature signature;
        signature.set(get_component_type<MaterialComponent>(e), true);
        signature.set(get_component_type<StaticMeshComponent>(e), true);
        register_global_system<Component>(e, signature);
    }

    void init(Engine *e)
    {
        auto &p = get_provider(e);
        const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

        auto dimensions = Vec2(mode->width, mode->height);
        init_render_texture(p.scene, mode->width, mode->height);
        p.scene.dimensions = dimensions;
        init_render_texture(p.game, mode->width, mode->height);
        p.game.dimensions = dimensions;
        init_input_texture(p.input_data, mode->width, mode->height);
        p.input_data.dimensions = dimensions;

        p.post_processing_shader = ShaderImporter::import_shader(ShaderSource("shaders/post_processing.glsl"));
        p.gaussian_blur_shader = ShaderImporter::import_shader(ShaderSource("shaders/gaussian_blur.glsl"));
        p.input_shader = ShaderImporter::import_engine_shader(ShaderImporter::EDITOR_INPUT);

        p.render_quad = MeshImporter::init_quad();
        p.skybox = MeshImporter::init_skybox();
    }

    void init_g_buffer(s32 width, s32 height)
    {
        assert("Unfinished");
    }

    void resize(Engine *e, s32 width, s32 height, u8 type)
    {
        auto &p = get_provider(e);
        if (type == GAME)
        {
            p.game.dimensions = Vec2(width, height);
        }
        else if (type == SCENE)
        {
            p.scene.dimensions = Vec2(width, height);
            p.input_data.dimensions = Vec2(width, height);
        }
    }

    void update_viewport_pos(Engine *e, s32 x, s32 y, u8 type)
    {
        // Change different positions in the provider
        if (type == GAME)
        {
            get_provider(e).game.position = Vec2(x, y);
        }
        else if (type == SCENE)
        {
            get_provider(e).scene.position = Vec2(x, y);
            get_provider(e).input_data.position = Vec2(x, y);
        }
    }

    static void perform_resize(ViewportData &data)
    {
        resize_framebuffer(data.render_fbo, data.dimensions.x, data.dimensions.y);

        if (is_valid_framebuffer(data.post_processed_fbo))
            resize_framebuffer(data.post_processed_fbo, data.dimensions.x, data.dimensions.y);

        for (u8 i = 0; i < 2; i++)
        {
            if (is_valid_framebuffer(data.ping_pong_blur_framebuffers[i]))
            {
                resize_framebuffer(data.ping_pong_blur_framebuffers[i], data.dimensions.x, data.dimensions.y);
            }
        }
    }

    static void draw_pass(Engine *e, const UpdateTick &tick)
    {
        // Get the providers
        auto &p = get_provider(e);
        const auto &camera_system_provider = CameraSystem::get_provider(e);
        const auto &light_system_provider = LightSystem::get_provider(e);

        u32 attachments[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};

        // Draw game onto render framebuffer
        {
            // Get the entities saved by other systems
            Entity camera = camera_system_provider.camera;

            // If no camera is in the scene, then something is wrong and we can't render
            if (camera != INVALID_ENTITY)
            {
                // This renders to the game scene, important for the editor
                bind_framebuffer(p.game.render_fbo);
                glDrawBuffers(2, attachments);

                // Clear the screen
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // Get the transform of the game camera that will actually be present
                // in an entity, can be non existent which is why we check earlier to ensure
                // that there actually is one
                auto &camera_transform = entity_get_component<TransformComponent>(e, camera);
                auto &camera_component = entity_get_component<CameraComponent>(e, camera);

                auto dimensions = get_dimensions(e, GAME);

                RenderContext context = {
                    .dimensions = get_dimensions(e, GAME),
                    .camera_transform = camera_transform,
                    .camera_component = camera_component,
                    .view_matrix = camera_transform.GetViewMatrix(),
                    .projection_matrix = camera_component.GetProjectionMatrix(dimensions.x, dimensions.y),
                };

                ShaderLoaderSystem::set_camera_uniform(e, {
                                                              .position = Vec4(camera_transform.position, 1),
                                                              .view_matrix = context.view_matrix,
                                                              .projection_matrix = context.projection_matrix,
                                                          });

                // Set the vieport dimensions to match that in the editor
                glViewport(0, 0, dimensions.x, dimensions.y);

                // Render both the skybox an the static meshes in the scene
                render_skybox(e, context, GAME);
                render_elements(e, context, GAME);

                // Unbind the frame buffer
                unbind_all_framebuffers();
            }
            else
            {
                fprintf(stderr, "NO CAMERA FOUND\n");
            }
        }

        // Draw scene onto render framebuffer
        if (tick.debug)
        {
            // Get the transform of the scene camera
            auto &camera_transform = camera_system_provider.scene_camera.transform_component;
            auto &camera_component = camera_system_provider.scene_camera.camera_component;

            // Always will have a scene camera, render to the editor scene view
            bind_framebuffer(p.scene.render_fbo);
            glDrawBuffers(2, attachments);

            // Clear the screen
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            auto dimensions = get_dimensions(e, SCENE);

            // Set the viewport to match that in the editor
            glViewport(0, 0, dimensions.x, dimensions.y);
            // Get the transform of the light
            // auto &light_transform = entity_get_component<TransformComponent>(light);

            RenderContext context = {
                .dimensions = get_dimensions(e, SCENE),
                .camera_transform = camera_transform,
                .camera_component = camera_component,
                .view_matrix = camera_transform.GetViewMatrix(),
                .projection_matrix = camera_component.GetProjectionMatrix(dimensions.x, dimensions.y),
            };

            ShaderLoaderSystem::set_camera_uniform(e, {
                                                          .position = Vec4(camera_transform.position, 1),
                                                          .view_matrix = context.view_matrix,
                                                          .projection_matrix = context.projection_matrix,
                                                      });

            // Render both the skybox and the static meshes in the scene
            render_skybox(e, context, SCENE);
            render_elements(e, context, SCENE);

            // Unbind the frame buffer
            unbind_all_framebuffers();

            // Render the elements again for mouse picking
            bind_framebuffer(p.input_data.render_fbo);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Set the viewport to match that in the editor
            glViewport(0, 0, get_dimensions(e, SCENE).x, get_dimensions(e, SCENE).y);

            render_element_input(e, context);

            // Render both the skybox and the static meshes in the scene
            unbind_all_framebuffers();
        }
    }

    static Texture &bloom_pass(Engine *e, ViewportData &data)
    {
        // Get the providers
        auto &p = get_provider(e);
        const auto &camera_system_provider = CameraSystem::get_provider(e);
        const auto &light_system_provider = LightSystem::get_provider(e);

        Entity camera = camera_system_provider.camera;

        bool horizontal = true;
        bool first_pass = true;

        f32 bloom_intensity = 7.0;
        u32 bloom_quality = 10;

        if (camera != INVALID_ENTITY)
        {
            auto &camera_component = entity_get_component<CameraComponent>(e, camera);
            bloom_intensity = camera_component.bloom_intensity;
            bloom_quality = camera_component.bloom_quality;
        }
        bind_shader(p.gaussian_blur_shader);
        set_uniform_1i(p.gaussian_blur_shader, "u_Render_texture", 0);
        set_uniform_1f(p.gaussian_blur_shader, "u_Blur_radius", bloom_intensity);

        glDisable(GL_DEPTH_TEST);

        for (u8 i = 0; i < bloom_quality; i++)
        {
            bind_framebuffer(data.ping_pong_blur_framebuffers[horizontal]);

            if (i < 2)
            {
                glViewport(0, 0, data.dimensions.x, data.dimensions.y);
                glClearColor(0.0, 0.0, 0.0, 0.0);
                glClear(GL_COLOR_BUFFER_BIT);
            }

            set_uniform_1i(p.gaussian_blur_shader, "u_Horizontal", horizontal);

            Texture texture;
            if (first_pass)
            {
                texture = get_framebuffer_color_texture(data.render_fbo, 1);
            }
            else
            {
                texture = get_framebuffer_color_texture(data.ping_pong_blur_framebuffers[!horizontal], 0);
            }
            bind_texture(texture, GL_TEXTURE0);

            draw_mesh(p.render_quad);

            horizontal = !horizontal;
            if (first_pass)
                first_pass = false;
        }

        unbind_all_framebuffers();
        glEnable(GL_DEPTH_TEST);

        return get_framebuffer_color_texture(data.ping_pong_blur_framebuffers[!horizontal], 0);
    }

    static void post_processing_pass(Engine *e, const UpdateTick &tick, Texture &game_bloom, Texture &scene_bloom)
    {
        // Get the providers
        auto &p = get_provider(e);
        const auto &camera_system_provider = CameraSystem::get_provider(e);
        const auto &light_system_provider = LightSystem::get_provider(e);

        Entity camera = camera_system_provider.camera;

        // Run post processing pass on the game
        {
            bind_framebuffer(p.game.post_processed_fbo);

            glViewport(0, 0, p.game.dimensions.x, p.game.dimensions.y);
            glClearColor(0.0, 0.0, 0.0, 0.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            bind_shader(p.post_processing_shader);
            set_uniform_1i(p.post_processing_shader, "u_Render_texture", 0);
            bind_texture(get_framebuffer_color_texture(p.game.render_fbo, 0), GL_TEXTURE0);

            set_uniform_1i(p.post_processing_shader, "u_Bloom_texture", 1);
            bind_texture(game_bloom, GL_TEXTURE1);

            if (camera != INVALID_ENTITY)
            {
                const auto &camera_component = entity_get_component<CameraComponent>(e, camera);
                set_uniform_1f(p.post_processing_shader, "u_Exposure", camera_component.exposure);
                set_uniform_1f(p.post_processing_shader, "u_Bloom_intensity", camera_component.bloom_intensity);
            }

            draw_mesh(p.render_quad);
        }
        if (tick.debug)
        {
            bind_framebuffer(p.scene.post_processed_fbo);

            glViewport(0, 0, p.scene.dimensions.x, p.scene.dimensions.y);
            glClearColor(0.0, 0.0, 0.0, 0.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            bind_shader(p.post_processing_shader);
            set_uniform_1i(p.post_processing_shader, "u_Render_texture", 0);

            bind_texture(get_framebuffer_color_texture(p.scene.render_fbo, 0), GL_TEXTURE0);

            set_uniform_1i(p.post_processing_shader, "u_Bloom_texture", 1);
            bind_texture(scene_bloom, GL_TEXTURE1);

            draw_mesh(p.render_quad);
        }

        unbind_all_framebuffers();
    }

    // Used in the actual update loop in main
    void update(Engine *e, const UpdateTick &meta_data)
    {
        auto &provider = get_provider(e);
        const auto &camera_system_provider = CameraSystem::get_provider(e);
        const auto &light_system_provider = LightSystem::get_provider(e);

        perform_resize(provider.scene);
        perform_resize(provider.game);
        perform_resize(provider.input_data);

        ShaderLoaderSystem::bind_camera_uniform(e);
        ShaderLoaderSystem::bind_directional_light_uniform(e);
        ShaderLoaderSystem::bind_point_lights_uniform(e);

        draw_pass(e, meta_data);
        auto game_bloom = bloom_pass(e, provider.game);
        auto scene_bloom = invalid_texture();
        if (meta_data.debug)
        {
            scene_bloom = bloom_pass(e, provider.scene);
        }
        post_processing_pass(e, meta_data, game_bloom, scene_bloom);

        // if (!meta_data.debug)
        // {
        // }

        // if (meta_data.debug)
        // {
        // }
    }

    // Render all of the static meshes in the scene
    void render_elements(Engine *e, const RenderContext &context, u8 type)
    {
        auto &provider = get_provider(e);
        glEnable(GL_BLEND);

        for (Entity entity : provider.entities)
        {
            MaterialComponent &material = entity_get_component<MaterialComponent>(e, entity);
            if (signature_contains(entity_get_signature(e, entity), get_component_type<SkyBoxComponent>(e)))
                continue;
            TransformComponent &transform = entity_get_component<TransformComponent>(e, entity);
            StaticMeshComponent &mesh = entity_get_component<StaticMeshComponent>(e, entity);
            Mesh mesh_obj = MeshLoaderSystem::get_mesh(e, mesh.source);
            if (is_valid_mesh(mesh_obj))
            {
                Renderer3D::ForwardRenderer::submit(e, material, transform.Matrix(), mesh_obj, context);
            }
        }
    }

    void render_element_input(Engine *e, const RenderContext &context)
    {
        auto &provider = get_provider(e);

        // Both have to be cleared before rendering to the input buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render all elements, except use the input shader
        for (Entity entity : provider.entities)
        {

            // All the components we need
            auto &transform = entity_get_component<TransformComponent>(e, entity);
            auto &mesh = entity_get_component<StaticMeshComponent>(e, entity);
            auto mesh_obj = MeshLoaderSystem::get_mesh(e, mesh.source);

            // If the mesh has loaded
            if (is_valid_mesh(mesh_obj))
            {
                // Use the input shader
                Shader shader = provider.input_shader;
                bind_shader(shader);

                // Set MVP uniforms
                set_uniform_matrix_4fv(shader, "model", glm::value_ptr(transform.Matrix()));
                set_uniform_matrix_4fv(shader, "projection", glm::value_ptr(context.camera_component.GetProjectionMatrix(context.dimensions.x, context.dimensions.y)));
                set_uniform_matrix_4fv(shader, "view", glm::value_ptr(context.camera_transform.GetViewMatrix()));

                // Get an RGB value that will be used to represent our object using an entity id
                int r = (entity & 0x000000FF) >> 0;
                int g = (entity & 0x0000FF00) >> 8;
                int b = (entity & 0x00FF0000) >> 16;

                // And use that as the color for our object which will single color shaded
                set_uniform_4f(shader, "color", Vec4(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f));

                // Draw the mesh
                draw_mesh(mesh_obj);
            }
        }
    }

    void render_skybox(Engine *e, const RenderContext &context, u8 type)
    {
        Entity camera = CameraSystem::get_provider(e).camera;
        if (!camera)
            return;
        Signature signature;
        signature.set(get_component_type<SkyBoxComponent>(e), true);
        signature.set(get_component_type<MaterialComponent>(e), true);

        Signature camera_signature = entity_get_signature(e, camera);

        // If the camera doesn't have both a skybox and material component, then we can't render a skybox
        if (entity_has_component<SkyBoxComponent>(e, camera) && entity_has_component<MaterialComponent>(e, camera))
        {
            glDepthFunc(GL_LEQUAL); // Ensure depth test passes when values are equal to
                                    // the depth buffer's content
            glDepthMask(GL_FALSE);  // Disable depth mask

            // Get the components needed to render
            auto &material_component = entity_get_component<MaterialComponent>(e, camera);
            auto &skybox_component = entity_get_component<SkyBoxComponent>(e, camera);

            Renderer3D::ForwardRenderer::bind_material(e, material_component, context.camera_transform.Matrix(), context, skybox_component.identifier);

            draw_mesh(get_provider(e).skybox);
            glDepthMask(GL_TRUE);
            glDepthFunc(GL_LESS); // Reset depth test
        }
    }

} // namespace Vultr::RenderSystem

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
        d.fbo = new_framebuffer(width, height);

        // Attach our color texture
        auto texture = generate_texture(GL_TEXTURE_2D);
        attach_color_texture_framebuffer(d.fbo, texture, 0, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);

        // Attach our depth/stencil rbo
        auto rbo = new_renderbuffer();
        attach_stencil_depth_renderbuffer_framebuffer(d.fbo, rbo);

        confirm_complete_framebuffer(d.fbo);
    }

    static void init_input_texture(ViewportData &d, u32 width, u32 height)
    {
        d.fbo = new_framebuffer(width, height);

        // Attach our color texture
        auto texture = generate_texture(GL_TEXTURE_2D);
        attach_color_texture_framebuffer(d.fbo, texture, 0, GL_RGBA16F, GL_RGBA, GL_FLOAT);

        // Attach our depth/stencil rbo
        auto rbo = new_renderbuffer();
        attach_stencil_depth_renderbuffer_framebuffer(d.fbo, rbo);

        confirm_complete_framebuffer(d.fbo);
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
        resize_framebuffer(data.fbo, data.dimensions.x, data.dimensions.y);
        // if (data.fbo.width != data.dimensions.x || data.fbo.height != data.dimensions.y)
        // {
        //     delete_framebuffer(data.fbo);
        //     init_render_texture(data, data.dimensions.x, data.dimensions.y);
        // }
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

        // Get the entities saved by other systems
        Entity camera = camera_system_provider.camera;

        // If no camera is in the scene, then something is wrong and we can't render
        if (camera != INVALID_ENTITY)
        {
            // This renders to the game scene, important for the editor
            bind_framebuffer(provider.game.fbo);

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

            // Update the gui system for the game
            // GUISystem::update(meta_data);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            // Unbind the frame buffer
            unbind_all_framebuffers();
        }
        else
        {
            std::cout << "NO CAMERA FOUND" << std::endl;
        }

        if (!meta_data.debug)
        {
            if (camera != INVALID_ENTITY)
            {
                auto &camera_component = entity_get_component<CameraComponent>(e, camera);
                if (camera_component.gamma_correction)
                {
                    glEnable(GL_FRAMEBUFFER_SRGB);
                }
            }
            const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            glViewport(0, 0, mode->width, mode->height);
            glClearColor(0.0, 0.0, 0.0, 0.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            bind_shader(provider.post_processing_shader);
            set_uniform_1i(provider.post_processing_shader, "renderedTexture", 0);
            bind_texture(get_framebuffer_color_texture(provider.game.fbo, 0), GL_TEXTURE0);
            draw_mesh(provider.render_quad);
            glDisable(GL_FRAMEBUFFER_SRGB);
        }

        if (meta_data.debug)
        {
            // Get the transform of the scene camera
            auto &camera_transform = camera_system_provider.scene_camera.transform_component;
            auto &camera_component = camera_system_provider.scene_camera.camera_component;

            // Always will have a scene camera, render to the editor scene view
            bind_framebuffer(provider.scene.fbo);

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
            bind_framebuffer(provider.input_data.fbo);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Set the viewport to match that in the editor
            glViewport(0, 0, get_dimensions(e, SCENE).x, get_dimensions(e, SCENE).y);

            render_element_input(e, context);

            // Render both the skybox and the static meshes in the scene
            unbind_all_framebuffers();
        }
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

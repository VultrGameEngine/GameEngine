#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <core/components/material_component.h>
#include <core/components/sky_box_component.h>
#include <core/components/static_mesh_component.h>
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
    static void render_skybox(u8 type);
    static void render_elements(u8 type);
    static void render_element_input();

    void register_system()
    {
        Signature signature;
        signature.set(get_component_type<MaterialComponent>(), true);
        signature.set(get_component_type<StaticMeshComponent>(), true);
        register_global_system<Component>(signature);
    }

    void init()
    {
        auto &p = get_provider();
        const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        generate_render_texture(p.scene, mode->width, mode->height);
        generate_render_texture(p.game, mode->width, mode->height);
        generate_input_render_texture(mode->width, mode->height);
        p.post_processing_shader = ShaderImporter::import_shader(ShaderSource("shaders/post_processing.glsl"));
        p.input_shader = ShaderImporter::import_engine_shader(ShaderImporter::EDITOR_INPUT);
        p.render_quad = MeshImporter::init_quad();
        p.skybox = MeshImporter::init_skybox();
    }

    void init_g_buffer(s32 width, s32 height)
    {
        assert("Unfinished");
    }

    void generate_render_texture(ViewportData &data, s32 width, s32 height)
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
        data.dimensions.x = width;
        data.dimensions.y = height;
    }

    void generate_input_render_texture(s32 width, s32 height)
    {
        auto &data = get_provider().input_data;
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
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        data.render_texture->FrameBufferTexture2D();

        data.rbo = new RenderBuffer(width, height);

        data.fbo->Unbind();
        data.dimensions.x = width;
        data.dimensions.y = height;
    }

    void resize(s32 width, s32 height, u8 type)
    {
        auto &provider = get_provider();
        auto p_dimensions = Vec2(width, height);
        // init_g_buffer(width, height);
        if (type == GAME)
        {
            if (provider.game.dimensions == p_dimensions)
                return;
            provider.game.dimensions = p_dimensions;
            generate_render_texture(provider.game, width, height);
        }
        else if (type == SCENE)
        {
            // Actual scene fbo
            if (provider.scene.dimensions == p_dimensions)
                return;
            provider.scene.dimensions = p_dimensions;
            generate_render_texture(provider.scene, width, height);

            // Mouse picking fbo
            if (provider.input_data.dimensions == Vec2(width, height))
                return;
            provider.input_data.dimensions = p_dimensions;
            generate_input_render_texture(width, height);
        }
    }

    void update_viewport_pos(s32 x, s32 y, u8 type)
    {
        // Change different positions in the provider
        if (type == GAME)
        {
            get_provider().game.position = Vec2(x, y);
        }
        else if (type == SCENE)
        {
            get_provider().scene.position = Vec2(x, y);
            get_provider().input_data.position = Vec2(x, y);
        }
    }

    // Used in the actual update loop in main
    void update(const UpdateTick &meta_data)
    {
        auto &provider = get_provider();
        const auto &camera_system_provider = CameraSystem::get_provider();
        const auto &light_system_provider = LightSystem::get_provider();

        // Get the entities saved by other systems
        Entity camera = camera_system_provider.camera;

        // If no camera is in the scene, then something is wrong and we can't render
        if (camera != INVALID_ENTITY)
        {
            // This renders to the game scene, important for the editor
            provider.game.fbo->Bind();

            // Clear the screen
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Get the transform of the game camera that will actually be present
            // in an entity, can be non existent which is why we check earlier to ensure
            // that there actually is one
            auto &camera_transform = entity_get_component<TransformComponent>(camera);
            auto &camera_component = entity_get_component<CameraComponent>(camera);

            RenderContext::SetContext(get_dimensions(GAME), camera_transform, camera_component);

            // Set the vieport dimensions to match that in the editor
            glViewport(0, 0, get_dimensions(GAME).x, get_dimensions(GAME).y);

            // Render both the skybox an the static meshes in the scene
            render_skybox(GAME);
            render_elements(GAME);

            // Update the gui system for the game
            // GUISystem::update(meta_data);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            // Unbind the frame buffer
            provider.game.fbo->Unbind();
        }
        else
        {
            std::cout << "NO CAMERA FOUND" << std::endl;
        }

        if (!meta_data.debug)
        {
            if (camera != INVALID_ENTITY)
            {
                auto &camera_component = entity_get_component<CameraComponent>(camera);
                if (camera_component.gamma_correction)
                {
                    glEnable(GL_FRAMEBUFFER_SRGB);
                }
            }
            const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            glViewport(0, 0, mode->width, mode->height);
            glClearColor(0.0, 0.0, 0.0, 0.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            provider.post_processing_shader->Bind();
            provider.post_processing_shader->SetUniform1i("renderedTexture", 0);
            provider.game.render_texture->Bind(GL_TEXTURE0);
            provider.render_quad->Draw();
            glDisable(GL_FRAMEBUFFER_SRGB);
        }

        if (meta_data.debug)
        {
            // Get the transform of the scene camera
            auto &camera_transform = camera_system_provider.scene_camera.transform_component;
            auto &camera_component = camera_system_provider.scene_camera.camera_component;

            // Always will have a scene camera, render to the editor scene view
            provider.scene.fbo->Bind();

            // Clear the screen
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Set the viewport to match that in the editor
            glViewport(0, 0, get_dimensions(SCENE).x, get_dimensions(SCENE).y);
            // Get the transform of the light
            // auto &light_transform = entity_get_component<TransformComponent>(light);

            RenderContext::SetContext(get_dimensions(SCENE), camera_transform, camera_component);

            // Render both the skybox and the static meshes in the scene
            render_skybox(SCENE);
            render_elements(SCENE);

            // Unbind the frame buffer
            provider.scene.fbo->Unbind();

            // Render the elements again for mouse picking
            provider.input_data.fbo->Bind();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Set the viewport to match that in the editor
            glViewport(0, 0, get_dimensions(SCENE).x, get_dimensions(SCENE).y);

            render_element_input();

            // Render both the skybox and the static meshes in the scene
            provider.input_data.fbo->Unbind();
        }
    }

    // Render all of the static meshes in the scene
    void render_elements(u8 type)
    {
        auto &provider = get_provider();
        glEnable(GL_BLEND);

        for (Entity entity : provider.entities)
        {
            MaterialComponent &material = entity_get_component<MaterialComponent>(entity);
            if (signature_contains(entity_get_signature(entity), get_component_type<SkyBoxComponent>()))
                continue;
            TransformComponent &transform = entity_get_component<TransformComponent>(entity);
            StaticMeshComponent &mesh = entity_get_component<StaticMeshComponent>(entity);
            Mesh *mesh_obj = MeshLoaderSystem::get_mesh(mesh.source);
            if (mesh_obj != nullptr)
            {
                Renderer3D::ForwardRenderer::Submit(material, transform.Matrix(), *mesh_obj);
            }
        }
    }

    void render_element_input()
    {
        auto &provider = get_provider();

        // Both have to be cleared before rendering to the input buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render all elements, except use the input shader
        for (Entity entity : provider.entities)
        {

            // All the components we need
            auto &transform = entity_get_component<TransformComponent>(entity);
            auto &mesh = entity_get_component<StaticMeshComponent>(entity);
            auto *mesh_obj = MeshLoaderSystem::get_mesh(mesh.source);

            // If the mesh has loaded
            if (mesh_obj != nullptr)
            {
                // Get the render context data
                const auto &context = RenderContext::GetContext();

                // Use the input shader
                Shader *shader = provider.input_shader;
                shader->Bind();

                // Set MVP uniforms
                shader->SetUniformMatrix4fv("model", glm::value_ptr(transform.Matrix()));
                shader->SetUniformMatrix4fv("projection", glm::value_ptr(context.camera_component.GetProjectionMatrix(context.dimensions.x, context.dimensions.y)));
                shader->SetUniformMatrix4fv("view", glm::value_ptr(context.camera_transform.GetViewMatrix()));

                // Get an RGB value that will be used to represent our object using an entity id
                int r = (entity & 0x000000FF) >> 0;
                int g = (entity & 0x0000FF00) >> 8;
                int b = (entity & 0x00FF0000) >> 16;

                // And use that as the color for our object which will single color shaded
                shader->SetUniform4f("color", Vec4(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f));

                // Draw the mesh
                mesh_obj->Draw();
            }
        }
    }

    void render_skybox(u8 type)
    {
        Entity camera = CameraSystem::get_provider().camera;
        if (!camera)
            return;
        Signature signature;
        signature.set(get_component_type<SkyBoxComponent>(), true);
        signature.set(get_component_type<MaterialComponent>(), true);

        Signature camera_signature = entity_get_signature(camera);

        // If the camera doesn't have both a skybox and material component, then we can't render a skybox
        if (entity_has_component<SkyBoxComponent>(camera) && entity_has_component<MaterialComponent>(camera))
        {
            glDepthFunc(GL_LEQUAL); // Ensure depth test passes when values are equal to
                                    // the depth buffer's content
            glDepthMask(GL_FALSE);  // Disable depth mask

            // Get the components needed to render
            auto &material_component = entity_get_component<MaterialComponent>(camera);
            auto &skybox_component = entity_get_component<SkyBoxComponent>(camera);

            Renderer3D::ForwardRenderer::BindMaterial(material_component, RenderContext::GetContext().camera_transform.Matrix(), skybox_component.identifier.c_str());

            get_provider().skybox->Draw();
            glDepthMask(GL_TRUE);
            glDepthFunc(GL_LESS); // Reset depth test
        }
    }

} // namespace Vultr::RenderSystem

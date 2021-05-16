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
        p.post_processing_shader = ShaderImporter::ImportShader("res/shaders/post_processing.glsl");
        p.render_quad = MeshImporter::InitQuad();
    }

    void init_g_buffer(int width, int height)
    {
        assert("Unfinished");
    }

    void generate_render_texture(ViewportData &data, int width, int height)
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

    void resize(int width, int height, u8 type)
    {
        auto &provider = get_provider();
        // init_g_buffer(width, height);
        if (type == GAME)
        {
            if (provider.game.dimensions == glm::vec2(width, height))
            {
                return;
            }
            provider.game.dimensions = glm::vec2(width, height);
            generate_render_texture(provider.game, width, height);
        }
        else if (type == SCENE)
        {
            if (provider.scene.dimensions == glm::vec2(width, height))
            {
                return;
            }
            provider.scene.dimensions = glm::vec2(width, height);
            generate_render_texture(provider.scene, width, height);
            // GenInputFB(width, height);
        }
    }

    void update_viewport_pos(int x, int y, u8 type)
    {
        if (type == GAME)
        {
            get_provider().game.position = glm::vec2(x, y);
        }
        else if (type == SCENE)
        {
            get_provider().scene.position = glm::vec2(x, y);
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
        Entity light = light_system_provider.light;

        // If there is no light then we cannot render
        if (light == INVALID_ENTITY)
            return;

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

            // Get the transform of the light
            auto &light_transform = entity_get_component<TransformComponent>(light);
            RenderContext::SetContext(get_dimensions(GAME), light_transform.position, camera_transform, camera_component);

            // Set the vieport dimensions to match that in the editor
            glViewport(0, 0, get_dimensions(GAME).x, get_dimensions(GAME).y);

            // Render both the skybox an the static meshes in the scene
            render_skybox(GAME);
            render_elements(GAME);

            // Update the gui system for the game
            GUISystem::update(meta_data);

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
            const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            glViewport(0, 0, mode->width, mode->height);
            glClearColor(0.0, 0.0, 0.0, 0.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            provider.post_processing_shader->Bind();
            provider.post_processing_shader->SetUniform1i("renderedTexture", 0);
            provider.game.render_texture->Bind(GL_TEXTURE0);
            provider.render_quad->Draw();
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
            auto &light_transform = entity_get_component<TransformComponent>(light);

            RenderContext::SetContext(get_dimensions(SCENE), light_transform.position, camera_transform, camera_component);

            // Render both the skybox and the static meshes in the scene
            render_skybox(SCENE);
            render_elements(SCENE);

            // Unbind the frame buffer
            provider.scene.fbo->Unbind();

            // provider.input_data.fb->Bind();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Set the viewport to match that in the editor
            glViewport(0, 0, get_dimensions(SCENE).x, get_dimensions(SCENE).y);

            // render_element_input();

            // Render both the skybox and the static meshes in the scene
            // provider.input_data.fb->Unbind();
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
            if (material.identifier != nullptr)
                continue;
            TransformComponent &transform = entity_get_component<TransformComponent>(entity);
            StaticMeshComponent &mesh = entity_get_component<StaticMeshComponent>(entity);
            Mesh *mesh_obj = MeshLoaderSystem::get_mesh(mesh.path);
            if (mesh_obj != nullptr)
            {
                Renderer3D::ForwardRenderer::Submit(material, transform.Matrix(), *mesh_obj);
            }
        }
    }

    void render_element_input()
    {
        auto &provider = get_provider();

        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // for (Entity entity : provider.entities)
        // {
        //     TransformComponent &transform = entity_get_component<TransformComponent>(entity);
        //     StaticMeshComponent &mesh = entity_get_component<StaticMeshComponent>(entity);
        //     Mesh *mesh_obj = mesh.GetMesh();
        //     if (mesh_obj != nullptr)
        //     {
        //         Shader *shader = provider.input_shader;
        //         shader->Bind();

        //         shader->SetUniformMatrix4fv("model", glm::value_ptr(transform.Matrix()));

        //         const RenderContext &context = RenderContext::GetContext();

        //         shader->SetUniformMatrix4fv("projection", glm::value_ptr(context.camera_component.GetProjectionMatrix(context.dimensions.x, context.dimensions.y)));
        //         shader->SetUniformMatrix4fv("view", glm::value_ptr(context.camera_transform.GetViewMatrix()));
        //         int r = (entity.id & 0x000000FF) >> 0;
        //         int g = (entity.id & 0x0000FF00) >> 8;
        //         int b = (entity.id & 0x00FF0000) >> 16;
        //         shader->SetUniform4f("color", glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f));
        //         mesh_obj->Draw();
        //     }
        // }
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
        if (!((camera_signature & signature) == signature))
            return;

        glDepthFunc(GL_LEQUAL); // Ensure depth test passes when values are equal to
                                // the depth buffer's content

        auto &material_component = entity_get_component<MaterialComponent>(camera);
        auto &skybox_component = entity_get_component<SkyBoxComponent>(camera);
        glDepthMask(GL_FALSE);
        Renderer3D::ForwardRenderer::BindMaterial(material_component, RenderContext::GetContext().camera_transform.Matrix());

        if (skybox_component.vbo == 0 || skybox_component.vao == 0)
        {
            glGenVertexArrays(1, &skybox_component.vao);
            glGenBuffers(1, &skybox_component.vbo);
            glBindVertexArray(skybox_component.vao);
            glBindBuffer(GL_ARRAY_BUFFER, skybox_component.vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * skybox_component.GetVertices().size(), &(skybox_component.GetVertices()[0]), GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        }

        glBindVertexArray(skybox_component.vao);
        glBindBuffer(GL_ARRAY_BUFFER, skybox_component.vbo);
        glDrawArrays(GL_TRIANGLES, 0, skybox_component.GetVertices().size());
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS); // Reset depth test
    }

} // namespace Vultr::RenderSystem

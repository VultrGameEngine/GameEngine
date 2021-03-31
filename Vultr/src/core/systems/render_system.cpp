#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <core/components/material_component.h>
#include <core/components/sky_box_component.h>
#include <core/components/static_mesh_component.h>
#include <core/system_providers/camera_system_provider.h>
#include <core/system_providers/light_system_provider.h>
#include <core/system_providers/render_system_provider.h>
#include <core/systems/gui_system.h>
#include <core/systems/render_system.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <rendering/renderers/forward_renderer_3d.h>

namespace Vultr
{
// Used in the actual update loop in main
void RenderSystem::Update(UpdateTick meta_data)
{
    RenderSystemProvider &provider = *RenderSystemProvider::Get();
    Entity camera = CameraSystemProvider::Get()->m_camera;
    Entity light = LightSystemProvider::Get()->light;
    if (light == -1)
        return;

    // If no camera is in the scene, then something is wrong and we can't render
    if (camera != -1)
    {
        // This renders to the game scene, important for the editor
        provider.game.fbo->Bind();

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Get the transform of the game camera that will actually be present
        // in an entity, can be non existent which is why we check earlier to ensure
        // that there actually is one
        auto &camera_transform = camera.GetComponent<TransformComponent>();
        auto &camera_component = camera.GetComponent<CameraComponent>();

        // Get the transform of the light
        auto &light_transform =
            LightSystemProvider::Get()->light.GetComponent<TransformComponent>();
        RenderContext::SetContext(provider.GetDimensions(GAME),
                                  light_transform.position, camera_transform,
                                  camera_component);

        // Set the vieport dimensions to match that in the editor
        glViewport(0, 0, provider.GetDimensions(GAME).x,
                   provider.GetDimensions(GAME).y);

        // Render both the skybox an the static meshes in the scene
        RenderSkybox(GAME);
        RenderElements(GAME);

        // Update the gui system for the game
        GUISystem::Update(meta_data);

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
        provider.post_processing_shader->Bind();
        provider.post_processing_shader->SetUniform1i("renderedTexture", 0);
        provider.game.render_texture->Bind(GL_TEXTURE0);
        provider.render_quad->Draw();
    }

    if (meta_data.debug)
    {
        // Get the transform of the scene camera
        auto &camera_transform =
            CameraSystemProvider::Get()->m_scene_camera.transform_component;
        auto &camera_component =
            CameraSystemProvider::Get()->m_scene_camera.camera_component;

        // Always will have a scene camera, render to the editor scene view
        provider.scene.fbo->Bind();

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set the viewport to match that in the editor
        glViewport(0, 0, provider.GetDimensions(SCENE).x,
                   provider.GetDimensions(SCENE).y);
        // Get the transform of the light
        auto &light_transform =
            LightSystemProvider::Get()->light.GetComponent<TransformComponent>();

        RenderContext::SetContext(provider.GetDimensions(SCENE),
                                  light_transform.position, camera_transform,
                                  camera_component);

        // Render both the skybox and the static meshes in the scene
        RenderSkybox(SCENE);
        RenderElements(SCENE);

        // Unbind the frame buffer
        provider.scene.fbo->Unbind();
    }
}

// Render all of the static meshes in the scene
void RenderSystem::RenderElements(unsigned int type)
{
    RenderSystemProvider &provider = *RenderSystemProvider::Get();

    for (Entity entity : provider.entities)
    {
        MaterialComponent &material = entity.GetComponent<MaterialComponent>();
        if (material.identifier != nullptr)
            continue;
        TransformComponent &transform = entity.GetComponent<TransformComponent>();
        StaticMeshComponent &mesh = entity.GetComponent<StaticMeshComponent>();

        Renderer3D::ForwardRenderer::Submit(material, transform.Matrix(),
                                            *mesh.GetMesh());
    }
}

void RenderSystem::RenderSkybox(unsigned int type)
{
    Entity camera = CameraSystemProvider::Get()->m_camera;
    if (camera == -1)
        return;
    Signature signature;
    signature.set(World::GetComponentType<SkyBoxComponent>(), true);
    signature.set(World::GetComponentType<MaterialComponent>(), true);
    if ((signature & camera.GetSignature()) != camera.GetSignature())
        return;
    glDepthFunc(GL_LEQUAL); // Ensure depth test passes when values are equal to
                            // the depth buffer's content

    auto &material_component = camera.GetComponent<MaterialComponent>();
    auto &skybox_component = camera.GetComponent<SkyBoxComponent>();
    glDepthMask(GL_FALSE);
    Renderer3D::ForwardRenderer::BindMaterial(
        material_component, RenderContext::GetContext().camera_transform.Matrix());

    if (skybox_component.vbo == 0 || skybox_component.vao == 0)
    {
        glGenVertexArrays(1, &skybox_component.vao);
        glGenBuffers(1, &skybox_component.vbo);
        glBindVertexArray(skybox_component.vao);
        glBindBuffer(GL_ARRAY_BUFFER, skybox_component.vbo);
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(glm::vec3) * skybox_component.GetVertices().size(),
                     &(skybox_component.GetVertices()[0]), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }

    glBindVertexArray(skybox_component.vao);
    glBindBuffer(GL_ARRAY_BUFFER, skybox_component.vbo);
    glDrawArrays(GL_TRIANGLES, 0, skybox_component.GetVertices().size());
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS); // Reset depth test
}

// Internal private helper methods
void RenderSystem::RegisterSystem()
{
    Signature signature;
    signature.set(World::GetComponentType<MaterialComponent>(), true);
    signature.set(World::GetComponentType<StaticMeshComponent>(), true);
    World::RegisterSystem<RenderSystemProvider>(signature);
}

} // namespace Vultr

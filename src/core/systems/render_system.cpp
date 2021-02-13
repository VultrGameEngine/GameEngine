#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <core/components/material_component.h>
#include <core/components/static_mesh_component.h>
#include <core/system_providers/camera_system_provider.h>
#include <core/system_providers/light_system_provider.h>
#include <core/system_providers/render_system_provider.h>
#include <core/systems/render_system.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <rendering/renderers/forward_renderer_3d.h>

namespace Brick3D
{
void RenderSystem::OnCreateEntity(Entity entity)
{
}

void RenderSystem::OnDestroyEntity(Entity entity)
{
}

// Used in the actual update loop in main
void RenderSystem::Update(UpdateTick meta_data)
{
    RenderSystemProvider &provider = RenderSystemProvider::Get();
    Entity camera = CameraSystemProvider::Get().m_camera;
    Entity light = LightSystemProvider::Get().light;
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
        auto &camera_transform = World::GetComponent<TransformComponent>(camera);
        auto &camera_component = World::GetComponent<CameraComponent>(camera);

        // Get the transform of the light
        auto &light_transform = World::GetComponent<TransformComponent>(
            LightSystemProvider::Get().light);
        RenderContext::SetContext(provider.GetDimensions(GAME),
                                  light_transform.position, camera_transform,
                                  camera_component);

        // Set the vieport dimensions to match that in the editor
        glViewport(0, 0, provider.GetDimensions(GAME).x,
                   provider.GetDimensions(GAME).y);

        // Render both the skybox an the static meshes in the scene
        // RenderSkybox(GAME, camera_transform, camera_component);
        RenderElements(GAME);

        // Unbind the frame buffer
        provider.game.fbo->Unbind();
    }
    else
    {
        std::cout << "NO CAMERA FOUND" << std::endl;
    }

    // Get the transform of the scene camera
    auto &camera_transform =
        CameraSystemProvider::Get().m_scene_camera.transform_component;
    auto &camera_component =
        CameraSystemProvider::Get().m_scene_camera.camera_component;

    // Always will have a scene camera, render to the editor scene view
    provider.scene.fbo->Bind();

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set the viewport to match that in the editor
    glViewport(0, 0, provider.GetDimensions(SCENE).x,
               provider.GetDimensions(SCENE).y);
    // Get the transform of the light
    auto &light_transform =
        World::GetComponent<TransformComponent>(LightSystemProvider::Get().light);

    RenderContext::SetContext(provider.GetDimensions(SCENE),
                              light_transform.position, camera_transform,
                              camera_component);

    // Render both the skybox and the static meshes in the scene
    // RenderSkybox(SCENE, camera_transform, camera_component);
    RenderElements(SCENE);

    // Unbind the frame buffer
    provider.scene.fbo->Unbind();
}

// Render all of the static meshes in the scene
void RenderSystem::RenderElements(unsigned int type)
{
    RenderSystemProvider &provider = RenderSystemProvider::Get();
    Entity camera = CameraSystemProvider::Get().m_camera;

    for (Entity entity : provider.entities)
    {
        MaterialComponent &material = World::GetComponent<MaterialComponent>(entity);
        TransformComponent &transform =
            World::GetComponent<TransformComponent>(entity);
        StaticMeshComponent &mesh = World::GetComponent<StaticMeshComponent>(entity);

        Renderer3D::ForwardRenderer::Submit(material, transform.Matrix(),
                                            *mesh.GetMesh());
    }

    // Create a render context
    // Update the renderer
    // renderer->DeferredGeometryPass(context);
    // renderer->StencilPass(context);
    // renderer->LightPass(context);
    // renderer->CompositePass(context);
    // renderer->ForwardRenderingPass(context);
}

void RenderSystem::RenderSkybox(unsigned int type)
{
    Entity camera = CameraSystemProvider::Get().m_camera;
    if (camera == -1)
        return;
    glDepthFunc(GL_LEQUAL); // Ensure depth test passes when values are equal to
                            // the depth buffer's content
    Signature skybox_signature;
    // skybox_signature.set(World::GetComponentType<SkyBoxComponent>(), true);
    // skybox_signature.set(World::GetComponentType<ShaderComponent>(), true);

    // // If the camera has a texture, that means it has a skybox
    // if ((World::GetSignature(camera) & skybox_signature) == skybox_signature)
    // {
    //     auto &shader_component = World::GetComponent<ShaderComponent>(camera);
    //     auto &skybox_component = World::GetComponent<SkyBoxComponent>(camera);
    //     LoadedTexture *texture =
    //         TextureLoaderSystem::Get()->GetTexture(skybox_component.identifier);
    //     // If we have loaded the bytes into memory
    //     if (texture != nullptr && texture->loaded == loaded)
    //     {
    //         glDepthMask(GL_FALSE);
    //         glActiveTexture(GL_TEXTURE0);
    //         glBindTexture(GL_TEXTURE_CUBE_MAP, texture->id);

    //         Shader *shader = shader_component.GetShader();
    //         if (shader == nullptr)
    //             return;
    //         shader->Bind();
    //         shader->SetUniform1i("skybox", 0);
    //         shader->SetUniformMatrix4fv(
    //             "view", glm::value_ptr(
    //                         glm::mat4(glm::mat3(camera_transform.GetViewMatrix()))));
    //         shader->SetUniformMatrix4fv(
    //             "projection", glm::value_ptr(camera_component.GetProjectionMatrix(
    //                               GetDimensions(type).x, GetDimensions(type).y)));

    //         if (skybox_component.vbo == 0 || skybox_component.vao == 0)
    //         {
    //             glGenVertexArrays(1, &skybox_component.vao);
    //             glGenBuffers(1, &skybox_component.vbo);
    //             glBindVertexArray(skybox_component.vao);
    //             glBindBuffer(GL_ARRAY_BUFFER, skybox_component.vbo);
    //             glBufferData(GL_ARRAY_BUFFER,
    //                          sizeof(glm::vec3) *
    //                              skybox_component.GetVertices().size(),
    //                          &(skybox_component.GetVertices()[0]),
    //                          GL_STATIC_DRAW);
    //             glEnableVertexAttribArray(0);
    //             glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    //         }

    //         glBindVertexArray(skybox_component.vao);
    //         glBindBuffer(GL_ARRAY_BUFFER, skybox_component.vbo);
    //         glDrawArrays(GL_TRIANGLES, 0, skybox_component.GetVertices().size());
    //         glDepthMask(GL_TRUE);
    //     }
    // }
    // glDepthFunc(GL_LESS); // Reset depth test
}

// Internal private helper methods
void RenderSystem::RegisterSystem()
{
    RenderSystemProvider &provider = RenderSystemProvider::Get();
    provider.signature.set(World::GetComponentType<MaterialComponent>(), true);
    provider.signature.set(World::GetComponentType<StaticMeshComponent>(), true);
    World::RegisterSystem<RenderSystem>(provider.signature);
}

} // namespace Brick3D

#include "../../../include/core/systems/render_system.h"
#include "../../../include/core/components/camera_component.h"
#include "../../../include/core/components/controller_component.h"
#include "../../../include/core/components/light_component.h"
#include "../../../include/core/components/shader_component.h"
#include "../../../include/core/components/sky_box_component.h"
#include "../../../include/core/components/static_mesh_cache.h"
#include "../../../include/core/components/static_mesh_component.h"
#include "../../../include/core/components/texture_cache.h"
#include "../../../include/core/components/texture_component.h"
#include "../../../include/core/components/transform_component.h"
#include "../../../include/core/systems/camera_system.h"
#include "../../../include/core/systems/light_system.h"
#include "../../../include/core/systems/mesh_loader_system.h"
#include "../../../include/core/systems/texture_loader_system.h"
#include "../../../include/ecs/component/component.hpp"
#include "../../../include/ecs/world/world.hpp"
#include "../../../include/editor/editor.hpp"
#include "../../../include/helpers/loading_state.h"
#include "../../../include/rendering/render_context.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

std::shared_ptr<RenderSystem> RenderSystem::Get() {
  static std::shared_ptr<RenderSystem> instance;
  if (instance == 0) {
    instance = RenderSystem::RegisterSystem();
    instance->renderer = new Renderer::Renderer3D();
  }
  return instance;
}

void RenderSystem::OnCreateEntity(Entity entity) {}

void RenderSystem::OnDestroyEntity(Entity entity) {
  auto &static_mesh_component =
      World::GetComponent<StaticMeshComponent>(entity);
}

// Used in the actual update loop in main
void RenderSystem::Update(float delta_time) {
  for (Entity entity : entities) {
    renderer->Register(entity);
  }

  Entity camera = CameraSystem::Get()->camera;
  Entity light = LightSystem::Get()->light;
  if (light == -1)
    return;

  // Get the transform of the scene camera
  auto &camera_transform =
      CameraSystem::Get()->scene_camera.transform_component;
  auto &camera_component = CameraSystem::Get()->scene_camera.camera_component;

  // If no camera is in the scene, then something is wrong and we can't render
  if (camera != -1) {
    // This renders to the game scene, important for the editor
    glBindFramebuffer(GL_FRAMEBUFFER, Get()->game.fbo);

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Get the transform of the game camera that will actually be present
    // in an entity, can be non existent which is why we check earlier to ensure
    // that there actually is one
    auto &camera_transform = World::GetComponent<TransformComponent>(camera);
    auto &camera_component = World::GetComponent<CameraComponent>(camera);

    // Set the vieport dimensions to match that in the editor
    glViewport(0, 0, game.dimensions.x, game.dimensions.y);

    // Render both the skybox an the static meshes in the scene
    RenderSkybox(GAME, camera_transform, camera_component);
    RenderElements(GAME, camera_transform, camera_component, light);

    // Unbind the frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  } else {
    std::cout << "NO CAMERA FOUND" << std::endl;
  }

  // Always will have a scene camera, render to the editor scene view
  glBindFramebuffer(GL_FRAMEBUFFER, Get()->scene.fbo);

  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Set the viewport to match that in the editor
  glViewport(0, 0, scene.dimensions.x, scene.dimensions.y);

  // Render both the skybox and the static meshes in the scene
  RenderSkybox(SCENE, camera_transform, camera_component);
  RenderElements(SCENE, camera_transform, camera_component, light);

  // Unbind the frame buffer
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// Render all of the static meshes in the scene
void RenderSystem::RenderElements(unsigned int type,
                                  TransformComponent camera_transform,
                                  CameraComponent camera_component,
                                  Entity light) {

  // Get the transform of the light
  auto &light_transform = World::GetComponent<TransformComponent>(light);

  // Update the renderer
  renderer->Update(Renderer::RenderContext{
      .dimensions = GetDimensions(type),
      .light_position = light_transform.position,
      .camera_transform = camera_transform,
      .camera_component = camera_component,
  });
}

void RenderSystem::RenderSkybox(unsigned int type,
                                TransformComponent camera_transform,
                                CameraComponent camera_component) {
  Entity camera = CameraSystem::Get()->camera;
  if (camera == -1)
    return;
  glDepthFunc(GL_LEQUAL); // Ensure depth test passes when values are equal to
                          // the depth buffer's content
  Signature skybox_signature;
  skybox_signature.set(World::GetComponentType<SkyBoxComponent>(), true);
  skybox_signature.set(World::GetComponentType<ShaderComponent>(), true);

  // If the camera has a texture, that means it has a skybox
  if ((World::GetSignature(camera) & skybox_signature) == skybox_signature) {
    auto &shader_component = World::GetComponent<ShaderComponent>(camera);
    auto &skybox_component = World::GetComponent<SkyBoxComponent>(camera);
    LoadedTexture *texture =
        TextureLoaderSystem::Get()->GetTexture(skybox_component.identifier);
    // If we have loaded the bytes into memory
    if (texture != nullptr && texture->loaded == loaded) {
      glDepthMask(GL_FALSE);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_CUBE_MAP, texture->id);

      Shader *shader = shader_component.GetShader();
      if (shader == nullptr)
        return;
      shader->Bind();
      shader->SetUniform1i("skybox", 0);
      shader->SetUniformMatrix4fv(
          "view", glm::value_ptr(
                      glm::mat4(glm::mat3(camera_transform.GetViewMatrix()))));
      shader->SetUniformMatrix4fv(
          "projection", glm::value_ptr(camera_component.GetProjectionMatrix(
                            GetDimensions(type).x, GetDimensions(type).y)));

      if (skybox_component.vbo == 0 || skybox_component.vao == 0) {
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
    }
  }
  glDepthFunc(GL_LESS); // Reset depth test
}

// Internal private helper methods
std::shared_ptr<RenderSystem> RenderSystem::RegisterSystem() {
  std::shared_ptr<RenderSystem> ptr = World::RegisterSystem<RenderSystem>();
  ptr->signature.set(World::GetComponentType<ShaderComponent>(), true);
  ptr->signature.set(World::GetComponentType<TextureComponent>(), true);
  ptr->signature.set(World::GetComponentType<StaticMeshComponent>(), true);

  World::SetSignature<RenderSystem>(ptr->signature);
  return ptr;
}

void RenderSystem::Resize(int width, int height, unsigned int type) {
  if (type == GAME) {
    if (Get()->game.dimensions == glm::vec2(width, height)) {
      return;
    }
    Get()->game.dimensions = glm::vec2(width, height);
    GenerateRenderTexture(&Get()->game.fbo, &Get()->game.render_texture,
                          &Get()->game.rbo, width, height);
  } else if (type == SCENE) {
    if (Get()->scene.dimensions == glm::vec2(width, height)) {
      return;
    }
    Get()->scene.dimensions = glm::vec2(width, height);
    GenerateRenderTexture(&Get()->scene.fbo, &Get()->scene.render_texture,
                          &Get()->scene.rbo, width, height);
  }
}

glm::vec2 RenderSystem::GetDimensions(unsigned int type) {
  if (type == SCENE) {
    return Get()->scene.dimensions;
  } else {
    return Get()->game.dimensions;
  }
}

void RenderSystem::GenerateRenderTexture(unsigned int *fbo,
                                         unsigned int *render_texture,
                                         unsigned int *rbo, int width,
                                         int height) {
  if (*fbo != 0) {
    glDeleteFramebuffers(1, fbo);
    glDeleteTextures(1, render_texture);
    glDeleteRenderbuffers(1, rbo);
  }
  glCreateFramebuffers(1, fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, *fbo);

  glCreateTextures(GL_TEXTURE_2D, 1, render_texture);
  glBindTexture(GL_TEXTURE_2D, *render_texture);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         *render_texture, 0);

  glCreateRenderbuffers(1, rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, *rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width,
                        height); // use a single renderbuffer object for both a
                                 // depth AND stencil buffer.
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, *rbo); // now actually attach it
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!"
              << std::endl;
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

#include "../../include/rendering/renderer_3d.h"
#include "../../include/core/components/shader_component.h"
#include "../../include/rendering/models/vertex.h"
#include <glm/gtc/type_ptr.hpp>

namespace Renderer {

Renderer3D::Renderer3D() {}

Renderer3D::~Renderer3D() {}

void Renderer3D::InitGBuffer(int width, int height) {
  g_buffer = new GBuffer();
  g_buffer->Init(width, height);

  if (render_quad.vao == 0) {
    InitQuad();
  }
  if (render_sphere.vao == 0) {
    InitSphere();
  }
  lighting_pass_shader = new Shader(
      ShaderLoaderSystem::CreateShader("res/shaders/lighting_pass.glsl"));

  stencil_pass_shader =
      new Shader(ShaderLoaderSystem::CreateShader("res/shaders/stencil.glsl"));

  composite_pass_shader = new Shader(
      ShaderLoaderSystem::CreateShader("res/shaders/composite_pass.glsl"));
}

void Renderer3D::LightPass(RenderContext context) {
  g_buffer->Unbind();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Bind the position, normal, and albedo textures
  g_buffer->BindWrite();
  g_buffer->SetWriteLight();
  g_buffer->BindPositionTexture();
  g_buffer->BindNormalTexture();
  lighting_pass_shader->Bind();
  // TODO make this dynamic
  lighting_pass_shader->SetUniform3f("lights[0].Position", glm::vec3(0, 0, 2));
  lighting_pass_shader->SetUniform3f("lights[0].Color", glm::vec3(1, 1, 1));
  const float linear = 0.7 / 10;
  const float quadratic = 1.8 / 10;
  lighting_pass_shader->SetUniform1f("lights[0].Linear", linear);
  lighting_pass_shader->SetUniform1f("lights[0].Quadratic", quadratic);

  lighting_pass_shader->SetUniform3f("viewPos",
                                     context.camera_transform.position);
  lighting_pass_shader->SetUniform1i("gPosition", 0);
  lighting_pass_shader->SetUniform1i("gNormal", 1);
  lighting_pass_shader->SetUniform1i("gAlbedoSpec", 2);

  glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_ONE, GL_ONE);

  DrawSphere();
  glDisable(GL_BLEND);
  g_buffer->UnbindWrite();
}

void Renderer3D::StencilPass(RenderContext context) {
  stencil_pass_shader->Bind();

  g_buffer->BindWrite();
  g_buffer->SetWriteNone();

  stencil_pass_shader->SetUniformMatrix4fv(
      "mView", glm::value_ptr(context.camera_transform.GetViewMatrix()));
  stencil_pass_shader->SetUniformMatrix4fv(
      "projection", glm::value_ptr(context.camera_component.GetProjectionMatrix(
                        context.dimensions.x, context.dimensions.y)));
  stencil_pass_shader->SetUniform3f("worldPos", glm::vec3(0, 0, 2));
  stencil_pass_shader->SetUniform1f("radius", 10);

  glEnable(GL_DEPTH_TEST);
  glStencilFunc(GL_ALWAYS, 0, 0);
  glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
  glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

  glClear(GL_STENCIL_BUFFER_BIT);

  DrawSphere();

  glDisable(GL_DEPTH_TEST);

  stencil_pass_shader->Unbind();
}

void Renderer3D::CompositePass(RenderContext context) {
  g_buffer->Unbind();

  g_buffer->BindPositionTexture();
  g_buffer->BindNormalTexture();
  g_buffer->BindColorSpecularTexture();
  g_buffer->BindLightSpecularTexture();

  composite_pass_shader->SetUniform1i("positionMap", 0);
  composite_pass_shader->SetUniform1i("normalMap", 1);
  composite_pass_shader->SetUniform1i("colorMap", 2);
  composite_pass_shader->SetUniform1i("lightMap", 3);
  glClear(GL_COLOR_BUFFER_BIT);

  DrawQuad();
}

void Renderer3D::ForwardRenderingPass(RenderContext context) {
  g_buffer->BindRead();
  g_buffer->UnbindWrite();
  glBlitFramebuffer(0, 0, context.dimensions.x, context.dimensions.y, 0, 0,
                    context.dimensions.x, context.dimensions.y,
                    GL_DEPTH_BUFFER_BIT, GL_NEAREST);
  g_buffer->Unbind();

  for (const auto &[shader_id, group] : render_groups_forward) {
    if (group != nullptr)
      group->Render(context, Forward);
  }
}

bool Renderer3D::Register(Entity entity) {
  ShaderComponent &shader_component =
      World::GetComponent<ShaderComponent>(entity);

  if (shader_component.GetShader() == nullptr) {
    return false;
  }

  std::unordered_map<unsigned int, RenderGroup *> &groups =
      shader_component.type == Forward ? render_groups_forward
                                       : render_groups_deferred;

  // If we don't have a render group for the shader that this entity
  // requires, create one
  if (groups.find(shader_component.GetShader()->GetID()) == groups.end()) {
    groups[shader_component.GetShader()->GetID()] =
        new RenderGroup(shader_component.GetShader());
  }

  // Register the entity to the render group
  groups[shader_component.GetShader()->GetID()]->RegisterEntity(entity);

  // Successfully added the render group
  return true;
}

void Renderer3D::DeferredGeometryPass(RenderContext context) {
  if (render_groups_deferred.size() == 0)
    return;
  if (g_buffer == nullptr)
    return;
  g_buffer->Bind();
  g_buffer->SetWritePNC();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  for (const auto &[shader_id, group] : render_groups_deferred) {
    if (group != nullptr)
      group->Render(context, Deferred);
  }
  g_buffer->Unbind();
}

void Renderer3D::Flush() {
  for (const auto &[shader_id, group] : render_groups_deferred) {
    delete group;
  }
  if (render_quad.vao != 0) {
    glDeleteVertexArrays(1, &render_quad.vao);
    glDeleteBuffers(1, &render_quad.vbo);
  }
}

void Renderer3D::InitQuad() {
  float quadVertices[] = {
      -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
      1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f,  -1.0f, 0.0f, 1.0f, 0.0f,
  };
  glGenVertexArrays(1, &render_quad.vao);
  glGenBuffers(1, &render_quad.vbo);
  glBindVertexArray(render_quad.vao);
  glBindBuffer(GL_ARRAY_BUFFER, render_quad.vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices,
               GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
}

void Renderer3D::InitSphere() {
  std::vector<glm::vec3> positions;
  std::vector<unsigned short> indices;
  float m = 5; // squares per face
  for (int i = 0; i <= m - 1; i += 1) {
    for (int j = 0; j <= m - 1; j += 1) {
      float sa = -1.0f + 2.0f * (i / m);
      float sb = -1.0f + 2.0f * ((i + 1) / m);
      float ta = -1.0f + 2.0f * (j / m);
      float tb = -1.0f + 2.0f * ((j + 1) / m);
      GLuint i = GLuint(positions.size());
      positions.push_back(glm::normalize(glm::vec3(sa, tb, 1.0f)));
      positions.push_back(glm::normalize(glm::vec3(sb, tb, 1.0f)));
      positions.push_back(glm::normalize(glm::vec3(sb, ta, 1.0f)));
      positions.push_back(glm::normalize(glm::vec3(sa, ta, 1.0f)));
      indices.push_back(i + 0);
      indices.push_back(i + 1);
      indices.push_back(i + 2);
      indices.push_back(i + 2);
      indices.push_back(i + 3);
      indices.push_back(i + 0);
      i += 4;
      positions.push_back(glm::normalize(glm::vec3(sa, ta, -1.0f)));
      positions.push_back(glm::normalize(glm::vec3(sb, ta, -1.0f)));
      positions.push_back(glm::normalize(glm::vec3(sb, tb, -1.0f)));
      positions.push_back(glm::normalize(glm::vec3(sa, tb, -1.0f)));
      indices.push_back(i + 0);
      indices.push_back(i + 1);
      indices.push_back(i + 2);
      indices.push_back(i + 2);
      indices.push_back(i + 3);
      indices.push_back(i + 0);
      i += 4;
      positions.push_back(glm::normalize(glm::vec3(sa, 1.0f, ta)));
      positions.push_back(glm::normalize(glm::vec3(sb, 1.0f, ta)));
      positions.push_back(glm::normalize(glm::vec3(sb, 1.0f, tb)));
      positions.push_back(glm::normalize(glm::vec3(sa, 1.0f, tb)));
      indices.push_back(i + 0);
      indices.push_back(i + 1);
      indices.push_back(i + 2);
      indices.push_back(i + 2);
      indices.push_back(i + 3);
      indices.push_back(i + 0);
      i += 4;
      positions.push_back(glm::normalize(glm::vec3(sa, -1.0f, tb)));
      positions.push_back(glm::normalize(glm::vec3(sb, -1.0f, tb)));
      positions.push_back(glm::normalize(glm::vec3(sb, -1.0f, ta)));
      positions.push_back(glm::normalize(glm::vec3(sa, -1.0f, ta)));
      indices.push_back(i + 0);
      indices.push_back(i + 1);
      indices.push_back(i + 2);
      indices.push_back(i + 2);
      indices.push_back(i + 3);
      indices.push_back(i + 0);
      i += 4;
      positions.push_back(glm::normalize(glm::vec3(-1.0f, sa, ta)));
      positions.push_back(glm::normalize(glm::vec3(-1.0f, sb, ta)));
      positions.push_back(glm::normalize(glm::vec3(-1.0f, sb, tb)));
      positions.push_back(glm::normalize(glm::vec3(-1.0f, sa, tb)));
      indices.push_back(i + 0);
      indices.push_back(i + 1);
      indices.push_back(i + 2);
      indices.push_back(i + 2);
      indices.push_back(i + 3);
      indices.push_back(i + 0);
      i += 4;
      positions.push_back(glm::normalize(glm::vec3(1.0f, sa, tb)));
      positions.push_back(glm::normalize(glm::vec3(1.0f, sb, tb)));
      positions.push_back(glm::normalize(glm::vec3(1.0f, sb, ta)));
      positions.push_back(glm::normalize(glm::vec3(1.0f, sa, ta)));
      indices.push_back(i + 0);
      indices.push_back(i + 1);
      indices.push_back(i + 2);
      indices.push_back(i + 2);
      indices.push_back(i + 3);
      indices.push_back(i + 0);
      i += 4;
    }
  }
  sphere_indices = indices.size();

  glGenVertexArrays(1, &render_sphere.vao);
  glGenBuffers(1, &render_sphere.vbo);
  glBindVertexArray(render_sphere.vao);
  glBindBuffer(GL_ARRAY_BUFFER, render_sphere.vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(),
               &positions[0], GL_STATIC_DRAW);
  glGenBuffers(1, &render_sphere.ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_sphere.ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * indices.size(),
               &indices[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3),
                        (const void *)0);
}

void Renderer3D::DrawSphere() {
  glBindVertexArray(render_sphere.vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_sphere.ibo);
  glDrawElements(GL_TRIANGLES, sphere_indices, GL_UNSIGNED_SHORT, (void *)0);
}

void Renderer3D::DrawQuad() {
  glBindVertexArray(render_quad.vao);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray(0);
}
} // namespace Renderer

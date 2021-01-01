#include "../../include/rendering/renderer_3d.h"
#include "../../include/core/components/shader_component.h"
#include "../../include/rendering/models/vertex.h"

namespace Renderer {

Renderer3D::Renderer3D() {}

Renderer3D::~Renderer3D() {}

void Renderer3D::InitGBuffer(int width, int height) {
  g_buffer = new GBuffer();
  g_buffer->Init(width, height);

  if (render_quad.vao == 0) {
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)(3 * sizeof(float)));
  }
  lighting_pass_shader = new Shader(
      ShaderLoaderSystem::CreateShader("res/shaders/lighting_pass.glsl"));
}

void Renderer3D::LightPass(glm::vec3 view_position) {
  g_buffer->Unbind();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Bind the position, normal, and albedo textures
  g_buffer->BindTextures();
  lighting_pass_shader->Bind();
  // TODO make this dynamic
  lighting_pass_shader->SetUniform3f("lights[0].Position", glm::vec3(4, 0, 0));
  lighting_pass_shader->SetUniform3f("lights[0].Color", glm::vec3(1, 0, 0));
  const float linear = 0.7;
  const float quadratic = 1.8;
  lighting_pass_shader->SetUniform1f("lights[0].Linear", linear);
  lighting_pass_shader->SetUniform1f("lights[0].Quadratic", quadratic);

  lighting_pass_shader->SetUniform3f("viewPos", view_position);
  lighting_pass_shader->SetUniform1i("gPosition", 0);
  lighting_pass_shader->SetUniform1i("gNormal", 1);
  lighting_pass_shader->SetUniform1i("gAlbedoSpec", 2);
  glBindVertexArray(render_quad.vao);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray(0);
  lighting_pass_shader->Unbind();
}

bool Renderer3D::Register(Entity entity) {
  ShaderComponent &shader_component =
      World::GetComponent<ShaderComponent>(entity);

  if (shader_component.GetShader() == nullptr) {
    return false;
  }

  // If we don't have a render group for the shader that this entity requires,
  // create one
  if (render_groups.find(shader_component.GetShader()->GetID()) ==
      render_groups.end()) {
    render_groups[shader_component.GetShader()->GetID()] =
        new RenderGroup(shader_component.GetShader());
  }

  // Register the entity to the render group
  render_groups[shader_component.GetShader()->GetID()]->RegisterEntity(entity);

  // Successfully added the render group
  return true;
}

void Renderer3D::Update(RenderContext context) {
  if (render_groups.size() == 0)
    return;
  if (g_buffer == nullptr)
    return;
  g_buffer->Bind();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  for (const auto &[shader_id, group] : render_groups) {
    if (group != nullptr)
      group->Render(context);
  }
  g_buffer->Unbind();
}

void Renderer3D::Flush() {
  for (const auto &[shader_id, group] : render_groups) {
    delete group;
  }
  if (render_quad.vao != 0) {
    glDeleteVertexArrays(1, &render_quad.vao);
    glDeleteBuffers(1, &render_quad.vbo);
  }
}

} // namespace Renderer

#include "../../include/rendering/render_group.h"
#include "../../include/core/components/shader_cache.h"
#include "../../include/core/components/shader_component.h"
#include "../../include/ecs/world/world.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace Renderer {
void RenderGroup::Render(RenderContext context) {

  // Bind the shader that will be used for rendering all the following meshes
  this->shader->Bind();

  // Set the view matrix and projection matrix, as they do not change between
  // meshes
  this->shader->SetUniformMatrix4fv(
      "view", glm::value_ptr(context.camera_component.view_matrix));

  this->shader->SetUniformMatrix4fv(
      "projection", glm::value_ptr(context.camera_component.GetProjectionMatrix(
                        context.dimensions.x, context.dimensions.y)));

  this->shader->SetUniform3f("lightPos", context.light_position);

  this->shader->SetUniform3f("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
  this->shader->SetUniform3f("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
  this->shader->SetUniform3f("viewPos", context.camera_transform.position);

  // Render all registered entities
  for (RenderEntity entity : entities) {
    LoadedStaticMesh *loaded_mesh = entity.GetMesh();
    // If the vao or vbo hasn't been loaded into the mesh, we need to create
    // them and set up all of the buffers
    if (loaded_mesh->vao == 0 || loaded_mesh->vbo == 0) {
      // Generate the vertex arrays
      glGenVertexArrays(1, &(loaded_mesh->vao));
      glBindVertexArray(loaded_mesh->vao);

      // Generate the index buffer
      glGenBuffers(1, &(loaded_mesh->ibo));
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, loaded_mesh->ibo);

      // Set the buffer data for the index buffer
      glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                   sizeof(unsigned short) * loaded_mesh->indices.size(),
                   &(loaded_mesh->indices[0]), GL_STATIC_DRAW);

      // Generate the vertex buffers
      glGenBuffers(1, &(loaded_mesh->vbo));
      glBindBuffer(GL_ARRAY_BUFFER, loaded_mesh->vbo);

      // Set the buffer data for the vertex buffer
      glBufferData(GL_ARRAY_BUFFER,
                   sizeof(Renderer::Vertex) * loaded_mesh->vertices.size(),
                   &(loaded_mesh->vertices[0]), GL_STATIC_DRAW);
      // Set the vertex attrib layout
      // 0: position
      // 1: normals
      // 2: uvs
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Renderer::Vertex),
                            (const void *)offsetof(Renderer::Vertex, position));

      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Renderer::Vertex),
                            (const void *)offsetof(Renderer::Vertex, normal));

      glEnableVertexAttribArray(2);
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Renderer::Vertex),
                            (const void *)offsetof(Renderer::Vertex, uv));
    }

    // Bind the entity's texture
    entity.GetTexture()->Bind(GL_TEXTURE0);

    TransformComponent &transform_component =
        World::GetComponent<TransformComponent>(entity.entity);

    this->shader->SetUniformMatrix4fv(
        "model", glm::value_ptr(transform_component.Matrix()));

    this->shader->SetUniform1i("textureSampler", 0);

    // Bind the vao and ibo
    glBindVertexArray(loaded_mesh->vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, loaded_mesh->ibo);

    // Draw the triangles
    glDrawElements(GL_TRIANGLES,
                   loaded_mesh->indices.size() * sizeof(unsigned short),
                   GL_UNSIGNED_SHORT, (void *)0);
  }
}

void RenderGroup::RegisterEntity(Entity entity) {
  ShaderComponent &shader_component =
      World::GetComponent<ShaderComponent>(entity);
  if (shader_component.shader != this->shader->GetID())
    return;

  TextureComponent &texture_component =
      World::GetComponent<TextureComponent>(entity);

  LoadedTexture *texture =
      TextureLoaderSystem::GetTexture(texture_component.path);
  if (texture == nullptr)
    return;

  // Generate the render entity
  RenderEntity render_entity;
  render_entity.entity = entity;
  entities.insert(render_entity);
}

} // namespace Renderer

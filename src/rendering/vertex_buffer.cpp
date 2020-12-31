#include "../../include/rendering/vertex_buffer.h"
#include "../../include/core/components/shader_component.h"
#include "../../include/core/systems/camera_system.h"
#include "../../include/core/systems/light_system.h"
#include "../../include/core/systems/mesh_loader_system.h"
#include "../../include/core/systems/render_system.h"
#include "../../include/core/systems/texture_loader_system.h"
#include "../../include/rendering/models/shader.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

namespace Renderer {

Mesh CreateMesh(unsigned int index_vb, unsigned int index_ib,
                LoadedStaticMesh *loaded_mesh) {
  Mesh mesh = {loaded_mesh, index_vb, index_ib};
  return mesh;
};

void VertexBuffer::Init() {
  // glCreateVertexArrays(1, &vertex_array_id);
  // glBindVertexArray(vertex_array_id);

  // glCreateBuffers(1, &vertex_buffer_id);
  // glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
  // glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * MAX_VERTICES, nullptr,
  //              GL_DYNAMIC_DRAW);

  // // Set the attrib of the vertex to be a vec3 of floats (the position)
  // glEnableVertexAttribArray(0);
  // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
  //                       (const void *)offsetof(Vertex, position));

  // // Set the attrib of the vertex to be a vec3 of floats (the normals)
  // glEnableVertexAttribArray(1);
  // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
  //                       (const void *)offsetof(Vertex, normal));

  // // Set the attrib of the vertex to be a vec2 of floats (the uvs)
  // glEnableVertexAttribArray(2);
  // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
  //                       (const void *)offsetof(Vertex, uv));

  // // Set the attrib of the vertex to be a vec4 of floats (the colors)
  // glEnableVertexAttribArray(3);
  // glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
  //                       (const void *)offsetof(Vertex, color));

  // // Set the attrib of the vertex to be a float (the texture id)
  // glEnableVertexAttribArray(4);
  // glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex),
  //                       (const void *)offsetof(Vertex, texture_id));

  // glEnableVertexAttribArray(5);
  // glEnableVertexAttribArray(6);
  // glEnableVertexAttribArray(7);
  // glEnableVertexAttribArray(8);
  // glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
  //                       (const void *)offsetof(Vertex, model_matrix));
  // glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
  //                       (const void *)(offsetof(Vertex, model_matrix) + 16));
  // glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
  //                       (const void *)(offsetof(Vertex, model_matrix) + 32));
  // glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
  //                       (const void *)(offsetof(Vertex, model_matrix) + 48));
}

void VertexBuffer::Update() {
  // // for (auto const &[entity, mesh] : entities) {
  // //   TransformComponent &transform_component =
  // //       World::GetComponent<TransformComponent>(entity);
  // //   for (int i = 0; i < mesh.mesh->vertices.size(); i++) {
  // //     Vertex *vertex = &vertex_buffer[i + mesh.index_in_vb];
  // //     if (vertex != nullptr) {
  // //     }
  // //   }
  // // }
  // if (shader == 0)
  //   return;

  // glUseProgram(shader);

  // // Get all of the uniforms of the shader component
  // GLuint View = glGetUniformLocation(shader, "view");
  // GLuint Projection = glGetUniformLocation(shader, "projection");
  // GLuint LightPosition = glGetUniformLocation(shader, "lightPos");
  // GLuint ViewPosition = glGetUniformLocation(shader, "viewPos");
  // GLuint LightColor = glGetUniformLocation(shader, "lightColor");
  // GLuint TextureSampler = glGetUniformLocation(shader, "textureSampler");

  // Entity camera = CameraSystem::Get()->camera;
  // if (camera == -1)
  //   return;
  // auto &camera_transform = World::GetComponent<TransformComponent>(camera);
  // auto &camera_component = World::GetComponent<CameraComponent>(camera);

  // // The view matrix
  // glUniformMatrix4fv(View, 1, GL_FALSE,
  //                    glm::value_ptr(camera_component.view_matrix));

  // // And the projection matrix
  // glUniformMatrix4fv(Projection, 1, GL_FALSE,
  //                    glm::value_ptr(camera_component.GetProjectionMatrix(
  //                        RenderSystem::GetDimensions(GAME).x,
  //                        RenderSystem::GetDimensions(GAME).y)));
  // Entity light = LightSystem::Get()->light;
  // if (light == -1)
  //   return;

  // TransformComponent &light_transform =
  //     World::GetComponent<TransformComponent>(light);

  // // Light position is set in its own uniform
  // glUniform3f(LightPosition, light_transform.position.x,
  //             light_transform.position.y, light_transform.position.z);

  // // And we set some other color values for the rendering
  // glUniform3f(LightColor, 1.0f, 1.0f, 1.0f);

  // // We also set the position of the camera, which can either be the scene
  // // camera or the actual entity that is a camera
  // glUniform3f(ViewPosition, camera_transform.position.x,
  //             camera_transform.position.y, camera_transform.position.z);

  // glActiveTexture(GL_TEXTURE0);
  // glBindTexture(GL_TEXTURE_2D, this->texture_id);
  // glUniform1i(TextureSampler, 0);

  // glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
  // // glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex_buffer),
  // vertex_buffer);

  // glBindVertexArray(vertex_array_id);
  // glDrawElements(GL_TRIANGLES, MAX_INDICES * sizeof(unsigned short),
  //                GL_UNSIGNED_SHORT, nullptr);
}

void VertexBuffer::Flush() {
  // glDeleteVertexArrays(1, &vertex_array_id);
  // glDeleteBuffers(1, &vertex_buffer_id);
}

bool VertexBuffer::RegisterEntity(Entity entity) {

  //  // Get all of the components of this entity that we will need
  //  StaticMeshComponent &static_mesh_component =
  //      World::GetComponent<StaticMeshComponent>(entity);
  //  TransformComponent &transform_component =
  //      World::GetComponent<TransformComponent>(entity);
  //  TextureComponent &texture_component =
  //      World::GetComponent<TextureComponent>(entity);
  //  ShaderComponent &shader_component =
  //      World::GetComponent<ShaderComponent>(entity);

  //  // If we have already registered the entity, do nothing
  //  if (entities.find(entity) == entities.end())
  //    return false;

  //  // TODO fix this shit
  //  if (shader_component.shader == 0)
  //    return false;

  //  // Get the loaded mesh
  //  LoadedStaticMesh *mesh =
  //      MeshLoaderSystem::GetMesh(static_mesh_component.path);

  //  // If the mesh has not loaded, then we can't do anything to render it
  //  if (mesh == nullptr || mesh->loaded != loaded)
  //    return false;

  //  // Get the loaded texture
  //  LoadedTexture *texture =
  //      TextureLoaderSystem::GetTexture(texture_component.path);

  //  // If the texture has not loaded, then we can't do anything to render it
  //  if (texture == nullptr || texture->loaded != loaded)
  //    return false;

  //  this->texture_id = texture->id;

  //  // If we have reached the max number of textures in this vertex buffer and
  //  // we don't have the texture id requested, then we don't have room to bind
  //  // another one an thus can't do anything
  //  if (textures.size() >= GetMaxTextures() &&
  //      textures.find(texture->id) == textures.end())
  //    return false;

  //  // Allocate memory for local vertices which will be added to the global
  //  vertex
  //  // buffer
  //  Vertex *vertices = (Vertex *)malloc(mesh->vertices.size() *
  //  sizeof(Vertex));

  //  // For all of the vertices in the loaded mesh, add all of the vertices to
  //  our
  //  // own buffer
  //  //
  //  // TODO make the color value not constant white
  //  for (int i = 0; i < mesh->vertices.size(); i++) {
  //    vertices[i] = Vertex(mesh->vertices.at(i), mesh->normals.at(i),
  //                         mesh->uvs.at(i), glm::vec4(1, 1, 1, 1),
  //                         texture->id);
  //    vertices[i].UpdateTransform(transform_component.Matrix());
  //  }

  //  // Allocate memory for local vertices which will be added to the global
  //  index
  //  // buffer
  //  unsigned short *indices =
  //      (unsigned short *)malloc(mesh->indices.size() * sizeof(short));

  //  // For all of the indices in the loaded mesh, add them to our own buffer
  //  for (int i = 0; i < mesh->indices.size(); i++) {
  //    indices[i] = mesh->indices.at(i);
  //  }

  //  // Create a mesh struct which will contain all of the data that we need to
  //  // reference and update the buffer later
  //  Mesh local_mesh = CreateMesh(vertex_location, index_location, mesh);

  //  // If for whatever reason we couldn't add the vertices to this vertex
  //  buffer,
  //  // then we return false and clean up
  //  bool result = AddVertices(vertices, indices);

  //  // Clean up arrays
  //  delete vertices;
  //  delete indices;

  //  // If for whatever reason we couldn't add the vertices to this index
  //  buffer,
  //  // then we return false
  //  if (!result)
  //    return false;

  //  // Then we store that in our map
  //  entities[entity] = local_mesh;

  //  // Successfully added all of the data to our buffer
  //  return true;
}

bool VertexBuffer::AddVertices(Vertex *vertices, unsigned short *indices) {
  // // Determine how much space we have left in the vertex buffer
  // unsigned int remaining_space_vertex = MAX_VERTICES - vertex_location;

  // // Determine how much space we need for the new data
  // size_t number_of_vertices = sizeof(vertices) / sizeof(Vertex);

  // // If there isn't enough space in the buffer, then we can't do anything''
  // if (number_of_vertices > remaining_space_vertex)
  //   return false;

  // // Determine how much space we hve left in the index buffer
  // unsigned int remaining_space_index = MAX_INDICES - index_location;

  // // Determine how much space we need for the new data
  // size_t number_of_indices = sizeof(indices) / sizeof(unsigned short);

  // // If there isn't enough space in the buffer, then we can't do anything''
  // if (number_of_indices > remaining_space_index)
  //   return false;

  // // Add all of our new data to the buffers
  // for (int i = 0; i < number_of_vertices; i++) {
  //   vertex_buffer[i + vertex_location] = vertices[i];
  // }

  // for (int i = 0; i < number_of_indices; i++) {
  //   index_buffer[i + index_location] = indices[i] + vertex_location;
  // }

  // // Update the current locations
  // vertex_location += number_of_vertices;
  // index_location += number_of_indices;

  // // Successfully added all of the data to the buffers
  // return true;
}

}; // namespace Renderer

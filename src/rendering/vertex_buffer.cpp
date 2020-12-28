#include "../../include/rendering/vertex_buffer.h"
#include "../../include/core/systems/mesh_loader_system.h"
#include "../../include/core/systems/texture_loader_system.h"
#include "GL/gl.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace Renderer {

void VertexBuffer::Init() {
  glCreateVertexArrays(1, &vertex_array_id);
  glBindVertexArray(vertex_array_id);

  glCreateBuffers(1, &vertex_buffer_id);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * MAX_VERTICES, nullptr, GL_DYNAMIC_DRAW);

  // Set the attrib of the vertex to be a vec3 of floats (the position)
  glEnableVertexAttribArray(vertex_buffer_id, 0);
  glVertexAttribPointer(0, 3, GL_FLOAT, )


}

void VertexBuffer::Flush() {

}

bool VertexBuffer::RegisterEntity(Entity entity) {

  // Get all of the components of this entity that we will need
  StaticMeshComponent &static_mesh_component =
      World::GetComponent<StaticMeshComponent>(entity);
  TransformComponent &transform_component =
      World::GetComponent<TransformComponent>(entity);
  TextureComponent &texture_component =
      World::GetComponent<TextureComponent>(entity);

  // If we don't have any textures, that means we haven't even called begin
  // batch
  if (this->textures.size() == 0)
    return false;

  // If we have already registered the entity, do nothing
  if (entities.find(entity) != entities.end())
    return false;

  // Get the loaded mesh
  LoadedStaticMesh *mesh =
      MeshLoaderSystem::GetMesh(static_mesh_component.path);

  // If the mesh has not loaded, then we can't do anything to render it
  if (mesh == nullptr || mesh->loaded != loaded)
    return false;

  // Get the loaded texture
  LoadedTexture *texture =
      TextureLoaderSystem::GetTexture(texture_component.path);

  // If the texture has not loaded, then we can't do anything to render it
  if (texture == nullptr || texture->loaded != loaded)
    return false;

  // Allocate memory for local vertices which will be added to the global vertex
  // buffer
  Vertex *vertices = (Vertex *)malloc(mesh->vertices.size() * sizeof(Vertex));

  // For all of the vertices in the loaded mesh, add all of the vertices to our
  // own buffer
  //
  // TODO make the color value not constant white
  for (int i = 0; i < mesh->vertices.size(); i++) {
    vertices[i] = Vertex(mesh->vertices.at(i), mesh->normals.at(i),
                         mesh->uvs.at(i), glm::vec4(1, 1, 1, 1), texture->id);
  }

  // Allocate memory for local vertices which will be added to the global index
  // buffer
  unsigned short *indices =
      (unsigned short *)malloc(mesh->indices.size() * sizeof(short));

  // For all of the indices in the loaded mesh, add them to our own buffer
  for (int i = 0; i < mesh->indices.size(); i++) {
    indices[i] = mesh->indices.at(i);
  }

  // Create a mesh struct which will contain all of the data that we need to
  // reference and update the buffer later
  Mesh local_mesh = Mesh(vertex_location, index_location, mesh);

  // If for whatever reason we couldn't add the vertices to this vertex buffer,
  // then we return false and clean up
  bool result = AddVertices(vertices, indices);

  // Clean up arrays
  delete vertices;
  delete indices;

  // If for whatever reason we couldn't add the vertices to this index buffer,
  // then we return false
  if (!result)
    return false;

  // Then we store that in our map
  entities[entity] = local_mesh;

  // Successfully added all of the data to our buffer
  return true;
}

bool VertexBuffer::AddVertices(Vertex *vertices, unsigned short *indices) {
  // Determine how much space we have left in the vertex buffer
  unsigned int remaining_space_vertex = MAX_VERTICES - vertex_location;

  // Determine how much space we need for the new data
  size_t number_of_vertices = sizeof(vertices) / sizeof(Vertex);

  // If there isn't enough space in the buffer, then we can't do anything''
  if (number_of_vertices < remaining_space_vertex)
    return false;

  // Determine how much space we hve left in the index buffer
  unsigned int remaining_space_index = MAX_INDICES - index_location;

  // Determine how much space we need for the new data
  size_t number_of_indices = sizeof(indices) / sizeof(unsigned short);

  // If there isn't enough space in the buffer, then we can't do anything''
  if (number_of_indices < remaining_space_index)
    return false;

  // Add all of our new data to the buffers
  for (int i = 0; i < number_of_vertices; i++) {
    vertex_buffer[i + vertex_location] = vertices[i];
  }

  for (int i = 0; i < number_of_indices; i++) {
    index_buffer[i + index_location] = indices[i] + vertex_location;
  }

  // Update the current locations
  vertex_location += number_of_vertices;
  index_location += number_of_indices;

  // Successfully added all of the data to the buffers
  return true;
}

}; // namespace Renderer

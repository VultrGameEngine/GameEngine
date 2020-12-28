#pragma once
#include <glm/glm.hpp>

struct Vertex {

  // Vertex properties that are in the vertex buffer
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 uv;
  glm::vec4 color;

  // The texture of this model, remains static and does not change
  int texture_id;

  // Is global for all vertices of the mesh, and is generated from the
  // TransformComponent
  glm::mat4 model_matrix;

  // Constructor for a vertex, which will set all of the model values for the
  // vertices in the model
  // NOTE this will NOT set the model matrix as that is updated every frame
  // based on the TransformComponent
  Vertex(glm::vec3 mesh_position, glm::vec3 mesh_normal, glm::vec2 mesh_uv,
         glm::vec4 mesh_color, int texture)
      : position(mesh_position), normal(mesh_normal), uv(mesh_uv),
        color(mesh_color), texture_id(texture) {}

  inline void UpdateTransform(glm::mat4 matrix) { this->model_matrix = matrix; }
};

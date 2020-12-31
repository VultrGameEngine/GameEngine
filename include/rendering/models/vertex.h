#pragma once
#include <glm/glm.hpp>

namespace Renderer {

struct Vertex {

  // Vertex properties that are in the vertex buffer
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 uv;

  // Constructor for a vertex, which will set all of the model values for the
  // vertices in the model
  // NOTE this will NOT set the model matrix as that is updated every frame
  // based on the TransformComponent
  Vertex(glm::vec3 mesh_position = glm::vec3(0, 0, 0),
         glm::vec3 mesh_normal = glm::vec3(0, 0, 0),
         glm::vec2 mesh_uv = glm::vec2(0, 0))
      : position(mesh_position), normal(mesh_normal), uv(mesh_uv) {}
};

} // namespace Renderer

#pragma once
#include <glm/glm.hpp>

namespace Renderer {

struct Vertex {

  // Vertex properties that are in the vertex buffer
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 uv;

  Vertex(glm::vec3 mesh_position = glm::vec3(0, 0, 0),
         glm::vec3 mesh_normal = glm::vec3(0, 0, 0),
         glm::vec2 mesh_uv = glm::vec2(0, 0))
      : position(mesh_position), normal(mesh_normal), uv(mesh_uv) {}
};

} // namespace Renderer

#pragma once
#include <glm/glm.hpp>

struct Vertex {
  glm::vec3 position;
  glm::vec4 color;
  int texture_id;
  glm::mat4 model_matrix;
};

#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
struct TransformComponent {
  glm::vec3 position = glm::vec3(0, 0, 0);
  glm::quat rotation = glm::quat(1, 0, 0, 0);
  glm::vec3 scale = glm::vec3(1, 1, 1);

  inline glm::vec3 Forward() { return rotation * glm::vec3(0, 0, -1); }

  inline glm::vec3 Right() { return rotation * glm::vec3(1, 0, 0); }
  inline glm::vec3 Up() { return glm::vec3(0, 1, 0); }

  inline glm::mat4 Matrix() {
    glm::mat4 scaling_matrix = glm::scale(scale);
    glm::mat4 rotation_matrix = glm::toMat4(rotation);
    glm::mat4 translate_matrix = glm::translate(position);
    return translate_matrix * rotation_matrix * scaling_matrix;
  }

  inline glm::mat4 GetViewMatrix() {
    return glm::lookAt(position, position + Forward(), glm::vec3(0, 1, 0));
  }
};

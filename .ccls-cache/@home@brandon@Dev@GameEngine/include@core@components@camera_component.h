#pragma once
#include <glm/glm.hpp>

struct CameraComponent {
  bool enabled = false;

  float fov = 45.0f;
  float znear = 0.1f;
  float zfar = 100.0f;

  unsigned int skybox = 0;

  glm::mat4 GetProjectionMatrix(float width, float height);
};

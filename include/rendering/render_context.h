#pragma once
#include "../core/components/camera_component.h"
#include "../core/components/transform_component.h"
#include <glm/glm.hpp>

namespace Renderer {
struct RenderContext {
  glm::vec2 dimensions;
  glm::vec3 light_position;
  TransformComponent camera_transform;
  CameraComponent camera_component;
};
}; // namespace Renderer

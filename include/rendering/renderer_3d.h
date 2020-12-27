#pragma once
#include "../core/components/static_mesh_component.h"
#include "../core/components/transform_component.h"
#include "vertex_buffer.h"
#include <array>

namespace Renderer {

class Renderer3D {

public:
  // Basic methods for creating and destroying
  Renderer3D();
  ~Renderer3D();

  bool Register(TransformComponent transform_component,
                StaticMeshComponent static_mesh_component);

  void Update();
  void Flush();
};

} // namespace Renderer

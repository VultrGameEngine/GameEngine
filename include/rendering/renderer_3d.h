#pragma once
// #include "../core/components/camera_component.h"
#include "../core/components/static_mesh_component.h"
#include "render_context.h"
// #include "../core/components/transform_component.h"
#include "render_group.h"
#include "vertex_buffer.h"
#include <array>
#include <unordered_map>

namespace Renderer {

class Renderer3D {

public:
  // Basic methods for creating and destroying
  Renderer3D();
  ~Renderer3D();

  bool Register(Entity entity);

  void Update(RenderContext context);
  void Flush();

private:
  std::unordered_map<unsigned int, RenderGroup *> render_groups;
};

} // namespace Renderer

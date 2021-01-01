#pragma once
#include "../../include/rendering/models/g_buffer.h"
#include "../core/components/static_mesh_component.h"
#include "render_context.h"
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

  void InitGBuffer(int width, int height);
  void Update(RenderContext context);
  void LightPass(glm::vec3 view_position);
  void Flush();

private:
  std::unordered_map<unsigned int, RenderGroup *> render_groups;
  GBuffer *g_buffer;
  struct Quad {
    unsigned int vbo = 0;
    unsigned int vao = 0;
  } render_quad;
  Shader *lighting_pass_shader;
  const unsigned int NR_LIGHTS = 1;
  std::vector<glm::vec3> lightPositions;
  std::vector<glm::vec3> lightColors;
};

} // namespace Renderer

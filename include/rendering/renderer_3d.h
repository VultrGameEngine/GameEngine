#pragma once
#include "../../include/rendering/models/g_buffer.h"
#include "../core/components/static_mesh_component.h"
#include "render_context.h"
#include "render_group.h"
#include "render_type.h"
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
  void DeferredGeometryPass(RenderContext context);
  void LightPass(RenderContext context);
  void StencilPass(RenderContext context);
  void ForwardRenderingPass(RenderContext context);
  void SkyboxPass(RenderContext context);
  void CompositePass(RenderContext context);
  void Flush();

private:
  void InitSphere();
  void DrawSphere();

  void InitQuad();
  void DrawQuad();

  std::unordered_map<unsigned int, RenderGroup *> render_groups_deferred;
  std::unordered_map<unsigned int, RenderGroup *> render_groups_forward;
  GBuffer *g_buffer;
  struct Mesh {
    unsigned int vbo = 0;
    unsigned int vao = 0;

    // Not completely necessary
    unsigned int ibo = 0;
  } render_quad, render_sphere;
  Shader *lighting_pass_shader;
  Shader *stencil_pass_shader;
  Shader *composite_pass_shader;
  const unsigned int NR_LIGHTS = 1;
  std::vector<glm::vec3> lightPositions;
  std::vector<glm::vec3> lightColors;

  unsigned int sphere_indices = 0;
};

} // namespace Renderer

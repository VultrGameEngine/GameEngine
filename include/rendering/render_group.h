#pragma once
#include "../../include/core/components/camera_component.h"
#include "../../include/core/components/static_mesh_cache.h"
#include "../../include/core/components/static_mesh_component.h"
#include "../../include/core/components/transform_component.h"
#include "../../include/core/systems/mesh_loader_system.h"
#include "../core/systems/shader_loader_system.h"
#include "../core/systems/texture_loader_system.h"
#include "../ecs/entity/entity.hpp"
#include "../ecs/world/world.hpp"
#include "render_context.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <set>
#include <stdio.h>
#include <stdlib.h>

namespace Renderer {
struct RenderEntity {
  Entity entity;
  LoadedTexture *GetTexture() {
    // Get the texture component
    TextureComponent &component = World::GetComponent<TextureComponent>(entity);

    // Get the loaded texture
    LoadedTexture *texture = TextureLoaderSystem::GetTexture(component.path);
    return texture;
  }

  LoadedStaticMesh *GetMesh() {
    // Get the static mesh component
    StaticMeshComponent &static_mesh_component =
        World::GetComponent<StaticMeshComponent>(entity);

    // Get the loaded mesh
    LoadedStaticMesh *loaded_mesh =
        MeshLoaderSystem::GetMesh(static_mesh_component.path);

    return loaded_mesh;
  }

  RenderEntity() {}
  ~RenderEntity() {}
};

inline bool operator<(const RenderEntity &lhs, const RenderEntity &rhs) {
  return lhs.entity < rhs.entity;
}

class RenderGroup {
public:
  RenderGroup(Shader *input_shader) : shader(input_shader) {}

  ~RenderGroup() { delete shader; }

  void Render(RenderContext context);
  void RegisterEntity(Entity entity);

  inline static int GetMaxTextures() {
    int max_textures = 0;

    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_textures);
    return max_textures;
  }

private:
  std::set<RenderEntity> entities;
  Shader *shader;
};
} // namespace Renderer

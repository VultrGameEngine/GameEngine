#pragma once
#include "../core/components/static_mesh_cache.h"
#include "../core/components/static_mesh_component.h"
#include "../core/components/transform_component.h"
#include "../ecs/entity/entity.hpp"
#include "models/vertex.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <array>
#include <unordered_map>
#define MAX_QUADS 1000
#define MAX_VERTICES 4000
#define MAX_INDICES 6000

namespace Renderer {

struct Mesh {
  LoadedStaticMesh *mesh;
  // Represent the index into the vertex buffer at which this
  // mesh's elements appear
  unsigned int index_in_vb;

  // Represent the index into the index buffer at which this
  // mesh's elements appear
  unsigned int index_in_ib;

  Mesh(unsigned int index_vb, unsigned int index_ib,
       LoadedStaticMesh *loaded_mesh)
      : index_in_vb(index_vb), index_in_ib(index_ib), mesh(loaded_mesh){};
};

class VertexBuffer {
public:
  bool Init(std::vector<unsigned int> textures);

  // Register a mesh to this renderer to update every frame
  bool RegisterEntity(Entity entity);

  // Deregister a mesh for this renderer
  bool DeregisterEntity(Entity entity);

  inline static unsigned int GetMaxTextures() {
    unsigned int max_textures = 0;

    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_textures);
    return max_textures;
  }

private:
  // Setter functions which will allocate memory and ensure that everything is
  // in order
  //
  // call these instead of directly modifying the buffers
  int AddVertices(Vertex vertices[]);
  int AddIndices(unsigned int indices[]);

  // Vertex buffer using the defined max vertices
  Vertex vertex_buffer[MAX_VERTICES];

  // index buffer using the defined max indices
  unsigned short index_buffer[MAX_INDICES];

  // The textures associated with this vertex buffer
  std::vector<unsigned int> textures;

  // The entities to render in each update of the vertex buffer
  std::unordered_map<Entity, Mesh> entities;
};
}; // namespace Renderer

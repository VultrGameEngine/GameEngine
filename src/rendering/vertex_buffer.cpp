#include "../../include/rendering/vertex_buffer.h"
#include "../../include/core/systems/mesh_loader_system.h"

namespace Renderer {

bool VertexBuffer::Init(std::vector<unsigned int> textures) {
  if (textures.size() > GetMaxTextures())
    return false;
  this->textures = textures;
  return true;
}

bool VertexBuffer::RegisterEntity(Entity entity) {
  StaticMeshComponent &static_mesh_component =
      World::GetComponent<StaticMeshComponent>(entity);
  TransformComponent &transform_component =
      World::GetComponent<TransformComponent>(entity);

  // If we don't have any textures, that means we haven't even called begin
  // batch
  if (this->textures.size() == 0)
    return false;

  // Get the loaded mesh
  LoadedStaticMesh *mesh =
      MeshLoaderSystem::GetMesh(static_mesh_component.path);

  // If the mesh has not loaded, then we can't do anything to render it
  if (mesh == nullptr || mesh->loaded != loaded)
    return false;

  Vertex *vertices = (Vertex *)malloc(mesh->vertices.size() * sizeof(Vertex));

  return true;
}

}; // namespace Renderer

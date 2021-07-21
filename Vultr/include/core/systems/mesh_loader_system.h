// MeshLoaderSystem loads in all the specified meshes in all of the entities
// that have a StaticMeshComponent If there are entities that have a
// StaticMeshComponent and do not have their buffers or opengl data set then
// MeshLoaderSystem will do that with the specified path

#pragma once
#include <engine.hpp>
#include <ecs/entity/entity.hpp>

namespace Vultr::MeshLoaderSystem
{
    void register_system(Engine *e);
    void update(Engine *e);
    void on_create_entity(Engine *e, Entity entity);
} // namespace Vultr::MeshLoaderSystem

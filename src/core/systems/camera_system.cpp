#include "../../../include/core/systems/camera_system.h"
#include "../../../include/core/components/camera_component.h"
#include "../../../include/ecs/world/world.hpp"
#include "../../../include/editor/editor.hpp"

glm::mat4 CameraComponent::GetProjectionMatrix(float width, float height) {
  return glm::perspective(fov, width / height, znear, zfar);
}

void CameraSystem::OnCreateEntity(Entity entity) {
  auto &camera_component = World::GetComponent<CameraComponent>(entity);
  if (camera_component.enabled) {
    camera = entity;
  }
}

void CameraSystem::OnDestroyEntity(Entity entity) {
  if (camera == entity) {
    camera = -1;
    for (Entity camera_entity : entities) {
      auto &camera_component =
          World::GetComponent<CameraComponent>(camera_entity);
      if (camera_component.enabled) {
        camera = entity;
        break;
      }
    }
  }
}

std::shared_ptr<CameraSystem> CameraSystem::Get() {
  static std::shared_ptr<CameraSystem> instance;
  if (instance == 0) {
    instance = RegisterSystem();
  }
  return instance;
}

std::shared_ptr<CameraSystem> CameraSystem::RegisterSystem() {
  std::shared_ptr<CameraSystem> ptr = World::RegisterSystem<CameraSystem>();
  ptr->signature.set(World::GetComponentType<CameraComponent>(), true);
  ptr->signature.set(World::GetComponentType<TransformComponent>(), true);

  World::SetSignature<CameraSystem>(ptr->signature);
  return ptr;
}

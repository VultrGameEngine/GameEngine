#include "../../../include/core/systems/camera_system.h"
#include "../../../include/core/components/camera_component.h"
#include "../../../include/ecs/world/world.hpp"
#include "../../../include/editor/editor.hpp"

glm::mat4 CameraComponent::GetProjectionMatrix(float width, float height) {
  return glm::perspective(fov, width / height, znear, zfar);
}

void CameraSystem::Update() {
  for (Entity entity : entities) {
    auto &camera_component = World::GetComponent<CameraComponent>(entity);
    auto &transform_component = World::GetComponent<TransformComponent>(entity);
    if (camera_component.enabled) {
      camera_component.view_matrix = glm::lookAt(
          transform_component.position,
          transform_component.position + transform_component.Forward(),
          glm::vec3(0, 1, 0));
      camera = entity;
      break;
    }
  }
  auto &camera_component = CameraSystem::Get()->scene_camera.camera_component;
  auto &transform_component =
      CameraSystem::Get()->scene_camera.transform_component;
  camera_component.view_matrix =
      glm::lookAt(transform_component.position,
                  transform_component.position + transform_component.Forward(),
                  glm::vec3(0, 1, 0));
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

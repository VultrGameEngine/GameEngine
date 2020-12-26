#include "../../../include/core/systems/light_system.h"
#include "../../../include/core/components/light_component.h"
#include "../../../include/core/components/transform_component.h"
#include "../../../include/ecs/world/world.hpp"

void LightSystem::Update() {
  for (Entity entity : entities) {
    light = entity;
    return;
  }
  light = -1;
}

void LightSystem::OnCreateEntity(Entity entity) {
  std::cout << "New light" << std::endl;
}

std::shared_ptr<LightSystem> LightSystem::Get() {
  static std::shared_ptr<LightSystem> instance;
  if (instance == nullptr) {
    instance = RegisterSystem();
  }
  return instance;
}

std::shared_ptr<LightSystem> LightSystem::RegisterSystem() {
  std::shared_ptr<LightSystem> ptr = World::RegisterSystem<LightSystem>();

  ptr->signature.set(World::GetComponentType<LightComponent>(), true);
  ptr->signature.set(World::GetComponentType<TransformComponent>(), true);

  World::SetSignature<LightSystem>(ptr->signature);

  return ptr;
}

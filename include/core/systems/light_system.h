#pragma once
#include "../../ecs/system/system.hpp"
#include <glm/glm.hpp>

class LightSystem : public System {
public:
  static std::shared_ptr<LightSystem> Get();
  void Update();
  void OnCreateEntity(Entity entity) override;
  static std::shared_ptr<LightSystem> RegisterSystem();
  Entity light = -1;

private:
  Signature signature;
};

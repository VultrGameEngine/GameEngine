#pragma once
#include "../../ecs/system/system.hpp"
#include <glm/glm.hpp>

class LightSystem : public System
{
public:
    static std::shared_ptr<LightSystem> Get();
    void Update();
    void DestroyEntity(Entity entity) override{};
    static std::shared_ptr<LightSystem> RegisterSystem();
    Entity light = -1;
};
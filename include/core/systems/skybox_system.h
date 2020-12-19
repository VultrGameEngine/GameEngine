#pragma once
#include "../../ecs/system/system.hpp"
#include <glm/glm.hpp>

class SkyboxSystem : public System
{
public:
    static std::shared_ptr<SkyboxSystem> Get();
    void Update();
    void DestroyEntity(Entity entity) override{};
    static std::shared_ptr<SkyboxSystem> RegisterSystem();
};
#include "../../../include/core/systems/camera_system.h"
#include "../../../include/ecs/world/world.hpp"

void CameraSystem::Update()
{
}

std::shared_ptr<CameraSystem> CameraSystem::Get()
{
    static std::shared_ptr<CameraSystem> instance;
    if (instance == 0)
    {
        instance = RegisterSystem();
    }
    return instance;
}

std::shared_ptr<CameraSystem> CameraSystem::RegisterSystem()
{
    std::shared_ptr<CameraSystem> ptr = World::RegisterSystem<CameraSystem>();
    return ptr;
}
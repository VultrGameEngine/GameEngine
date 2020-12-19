#include "../../../include/core/systems/camera_system.h"
#include "../../../include/ecs/world/world.hpp"

void CameraSystem::Update()
{
    Signature camera_signature;
    camera_signature.set(World::GetComponentType<CameraComponent>(), true);
    camera_signature.set(World::GetComponentType<TransformComponent>(), true);
    for (Entity entity : World::GetEntities(camera_signature))
    {
        auto &camera_component = World::GetComponent<CameraComponent>(entity);
        auto &transform_component = World::GetComponent<TransformComponent>(entity);
        if (camera_component.enabled)
        {
            camera_component.view_matrix = glm::lookAt(transform_component.position, transform_component.position + transform_component.Forward(), glm::vec3(0, 1, 0));
            camera = entity;
            break;
        }
    }
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
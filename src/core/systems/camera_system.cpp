#include "../../../include/core/systems/camera_system.h"
#include "../../../include/ecs/world/world.hpp"

void CameraSystem::Update()
{
    Signature signature;
    signature.set(World::GetComponentType<CameraComponent>());
    signature.set(World::GetComponentType<TransformComponent>());
    for (Entity entity : World::GetEntities(signature))
    {
        CameraComponent &camera_component = World::GetComponent<CameraComponent>(entity);
        TransformComponent &transform_component = World::GetComponent<TransformComponent>(entity);

        // Projection matrix
        // camera_component.projection_matrix =
        camera_component.view_matrix = glm::lookAt(transform_component.position, transform_component.position, glm::vec3(0, 1, 0));
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
#include "../../../include/core/systems/camera_system.h"
#include "../../../include/ecs/world/world.hpp"

void CameraSystem::Update()
{
    Signature signature;
    signature.set(World::Get()->GetComponentType<CameraComponent>());
    signature.set(World::Get()->GetComponentType<TransformComponent>());
    for (Entity entity : World::Get()->GetEntities(signature))
    {
        CameraComponent &camera_component = World::Get()->GetComponent<CameraComponent>(entity);
        TransformComponent &transform_component = World::Get()->GetComponent<TransformComponent>(entity);

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
    std::shared_ptr<CameraSystem> ptr = World::Get()->RegisterSystem<CameraSystem>();
    return ptr;
}
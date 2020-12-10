#include "../../../include/core/systems/camera_system.h"
#include "../../../include/ecs/coordinator/coordinator.hpp"

std::shared_ptr<CameraSystem> CameraSystem::instance = 0;
Signature CameraSystem::signature = 0;

void CameraSystem::Update()
{
    for (Entity entity : Coordinator::Get()->GetEntities(signature))
    {
        CameraComponent &camera_component = Coordinator::Get()->GetComponent<CameraComponent>(entity);
        TransformComponent &transform_component = Coordinator::Get()->GetComponent<TransformComponent>(entity);

        // Projection matrix
        // camera_component.projection_matrix =
        camera_component.view_matrix = glm::lookAt(transform_component.position, transform_component.position, glm::vec3(0, 1, 0));
    }
}

std::shared_ptr<CameraSystem> CameraSystem::Get()
{
    if (instance == 0)
    {
        instance = RegisterSystem();
    }
    return instance;
}

std::shared_ptr<CameraSystem> CameraSystem::RegisterSystem()
{
    std::shared_ptr<CameraSystem> ptr = Coordinator::Get()->RegisterSystem<CameraSystem>();
    signature.set(Coordinator::Get()->GetComponentType<CameraComponent>());
    signature.set(Coordinator::Get()->GetComponentType<TransformComponent>());
    Coordinator::Get()->SetSystemSignature<CameraSystem>(signature);
    return ptr;
}
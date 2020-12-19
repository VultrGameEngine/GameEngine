// Camera system handles updating the main camera's matricies to match the transform

#pragma once
#include "../../ecs/system/system.hpp"
#include "../components/transform_component.h"
#include "../components/camera_component.h"
#include "../../ecs/world/world.hpp"
#include <memory>

class CameraSystem : public System
{
public:
    static std::shared_ptr<CameraSystem> Get();
    void Update();
    void DestroyEntity(Entity entity) override{};
    static std::shared_ptr<CameraSystem> RegisterSystem();
    Entity camera = -1;
    struct Camera
    {
        TransformComponent transform_component;
        CameraComponent camera_component;
    } scene_camera;
};
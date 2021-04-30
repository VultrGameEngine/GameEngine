#pragma once
#include <core/components/camera_component.h>
#include <core/components/controller_component.h>
#include <core/components/transform_component.h>
#include <ecs/system/system_provider.hpp>
#include <engine.hpp>

namespace Vultr::CameraSystemProvider
{
struct CameraSystemComponent
{
    // Member variables for state
    Entity m_camera = Entity(-1);
    struct Camera
    {
        TransformComponent m_transform_component;
        CameraComponent m_camera_component;
        ControllerComponent m_controller_component;
    } m_scene_camera;
};

std::shared_ptr<CameraSystemComponent> Get()
{
    return Engine::GetSystemProvider<CameraSystemComponent>();
}

} // namespace Vultr::CameraSystemProvider

VultrRegisterSystemProvider(Vultr::CameraSystemProvider::CameraSystemComponent)

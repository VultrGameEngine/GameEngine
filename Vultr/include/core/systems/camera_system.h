// Camera system handles updating the main camera's matricies to match the
// transform

#pragma once
#include <core/components/transform_component.h>
#include <core/system_providers/camera_system_provider.h>
#include <memory>

namespace Vultr::CameraSystem
{
    void register_system();
    void on_create_entity(Entity entity);
    void on_destroy_entity(Entity entity);

} // namespace Vultr::CameraSystem

// Camera system handles updating the main camera's matricies to match the
// transform

#pragma once
#include <engine.hpp>
#include <core/components/transform_component.h>
#include <core/system_providers/camera_system_provider.h>
#include <memory>

namespace Vultr::CameraSystem
{
    void register_system(Engine *e);
    void on_create_entity(Engine *e, Entity entity);
    void on_destroy_entity(Engine *e, Entity entity);

} // namespace Vultr::CameraSystem

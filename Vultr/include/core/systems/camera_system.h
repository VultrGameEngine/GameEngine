// Camera system handles updating the main camera's matricies to match the
// transform

#pragma once
#include <core/components/transform_component.h>
#include <core/system_providers/camera_system_provider.h>
#include <memory>

namespace Vultr::CameraSystem
{
void RegisterSystem();
void OnCreateEntity(Entity entity);
void OnDestroyEntity(Entity entity);

} // namespace Vultr::CameraSystem

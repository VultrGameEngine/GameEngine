#include <core/system_providers/camera_system_provider.h>
#include <core/systems/camera_system.h>

namespace Vultr
{

void CameraSystemProvider::OnCreateEntity(Entity entity)
{
    CameraSystem::OnCreateEntity(entity);
}

void CameraSystemProvider::OnDestroyEntity(Entity entity)
{
    CameraSystem::OnDestroyEntity(entity);
}
} // namespace Vultr

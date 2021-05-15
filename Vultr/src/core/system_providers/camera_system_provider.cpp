#include <core/system_providers/camera_system_provider.h>
#include <core/systems/camera_system.h>

namespace Vultr::CameraSystem
{
    Component &get_provider()
    {
        return *get_global_system_provider<Component>();
    }

} // namespace Vultr::CameraSystem
